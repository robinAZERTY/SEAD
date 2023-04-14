/*
  MPU9250.cpp
  Brian R Taylor
  brian.taylor@bolderflight.com

  Copyright (c) 2017 Bolder Flight Systems

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Arduino.h"
#include "MPU9250.h"

/* MPU9250 object, input the I2C bus and address */
MPU9250::MPU9250(TwoWire &bus, uint8_t address)
{
  _i2c = &bus;        // I2C bus
  _address = address; // I2C address
  _useSPI = false;    // set to use I2C
}

/* MPU9250 object, input the SPI bus and chip select pin */
MPU9250::MPU9250(SPIClass &bus, uint8_t csPin)
{
  _spi = &bus;    // SPI bus
  _csPin = csPin; // chip select pin
  _useSPI = true; // set to use SPI
}

/* starts communication with the MPU-9250 */
int MPU9250::begin()
{
  if (_useSPI)
  { // using SPI for communication
    // use low speed SPI for register setting
    _useSPIHS = false;
    // setting CS pin to output
    pinMode(_csPin, OUTPUT);
    // setting CS pin high
    digitalWrite(_csPin, HIGH);
    // begin SPI communication
    _spi->begin();
  }
  else
  { // using I2C for communication
    // starting the I2C bus
    _i2c->begin();
    // setting the I2C clock
    _i2c->setClock(_i2cRate);
  }
  // select clock source to gyro
  if (writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL) < 0)
  {
    return -1;
  }
  // enable I2C master mode
  if (writeRegister(USER_CTRL, I2C_MST_EN) < 0)
  {
    return -2;
  }
  // set the I2C bus speed to 400 kHz
  if (writeRegister(I2C_MST_CTRL, I2C_MST_CLK) < 0)
  {
    return -3;
  }
  // set AK8963 to Power Down
  writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN);
  // reset the MPU9250
  writeRegister(PWR_MGMNT_1, PWR_RESET);
  // wait for MPU-9250 to come back up
  delay(1);
  // reset the AK8963
  writeAK8963Register(AK8963_CNTL2, AK8963_RESET);
  // select clock source to gyro
  if (writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL) < 0)
  {
    return -4;
  }
  // check the WHO AM I byte, expected value is 0x71 (decimal 113) or 0x73 (decimal 115)
  if ((whoAmI() != 113) && (whoAmI() != 115))
  {
    return -5;
  }
  // enable accelerometer and gyro
  if (writeRegister(PWR_MGMNT_2, SEN_ENABLE) < 0)
  {
    return -6;
  }
  // setting accel range to 16G as default
  if (writeRegister(ACCEL_CONFIG, ACCEL_FS_SEL_16G) < 0)
  {
    return -7;
  }
  _accelScale = G * 16.0f / 32767.5f; // setting the accel scale to 16G
  _accelRange = ACCEL_RANGE_16G;
  // setting the gyro range to 2000DPS as default
  if (writeRegister(GYRO_CONFIG, GYRO_FS_SEL_2000DPS) < 0)
  {
    return -8;
  }
  _gyroScale = 2000.0f / 32767.5f * _d2r; // setting the gyro scale to 2000DPS
  _gyroRange = GYRO_RANGE_2000DPS;
  // setting bandwidth to 184Hz as default
  if (writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_184) < 0)
  {
    return -9;
  }
  if (writeRegister(CONFIG, GYRO_DLPF_184) < 0)
  { // setting gyro bandwidth to 184Hz
    return -10;
  }
  _bandwidth = DLPF_BANDWIDTH_184HZ;
  // setting the sample rate divider to 0 as default
  if (writeRegister(SMPDIV, 0x00) < 0)
  {
    return -11;
  }
  _srd = 0;
  // enable I2C master mode
  if (writeRegister(USER_CTRL, I2C_MST_EN) < 0)
  {
    return -12;
  }
  // set the I2C bus speed to 400 kHz
  if (writeRegister(I2C_MST_CTRL, I2C_MST_CLK) < 0)
  {
    return -13;
  }
  // check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
  if (whoAmIAK8963() != 72)
  {
    return -14;
  }
  /* get the magnetometer calibration */
  // set AK8963 to Power Down
  if (writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN) < 0)
  {
    return -15;
  }
  delay(100); // long wait between AK8963 mode changes
  // set AK8963 to FUSE ROM access
  if (writeAK8963Register(AK8963_CNTL1, AK8963_FUSE_ROM) < 0)
  {
    return -16;
  }
  delay(100); // long wait between AK8963 mode changes
  // read the AK8963 ASA registers and compute magnetometer scale factors
  readAK8963Registers(AK8963_ASA, 3, _buffer);
  _magScaleX = ((((float)_buffer[0]) - 128.0f) / (256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
  _magScaleY = ((((float)_buffer[1]) - 128.0f) / (256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
  _magScaleZ = ((((float)_buffer[2]) - 128.0f) / (256.0f) + 1.0f) * 4912.0f / 32760.0f; // micro Tesla
  // set AK8963 to Power Down
  if (writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN) < 0)
  {
    return -17;
  }
  delay(100); // long wait between AK8963 mode changes
  // set AK8963 to 16 bit resolution, 100 Hz update rate
  if (writeAK8963Register(AK8963_CNTL1, AK8963_CNT_MEAS2) < 0)
  {
    return -18;
  }
  delay(100); // long wait between AK8963 mode changes
  // select clock source to gyro
  if (writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL) < 0)
  {
    return -19;
  }
  // instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
  readAK8963Registers(AK8963_HXL, 7, _buffer);
  // estimate gyro bias
  // if (calibrateGyro() < 0) {
  //  return -20;
  //}
  // successful init, return 1
  Serial.println(EEPROM.begin(EEPROM_SIZE));
  return 1;
}

int MPU9250::read_from_flash_gyro_cal()
{
  gyr_calib_data gyr_readed;
  Serial.println("read Calibration From Flash");
  EEPROM.get(GYRO_CALIBRATION_ADDRESS, gyr_readed);
  // test if the data is valid (not yet implemented)
  _calData.gyr = gyr_readed;
  return 1;
}

int MPU9250::write_to_flash_gyro_cal()
{
  Serial.println("write gyro Calibration To Flash");
  EEPROM.put(GYRO_CALIBRATION_ADDRESS, _calData.gyr);
  EEPROM.commit();
  return 1;
}

int MPU9250::read_from_flash_accel_cal()
{
  acc_calib_data accel_readed;
  Serial.println("read accel Calibration From Flash");
  EEPROM.get(ACC_CALIBRATION_ADDRESS, accel_readed);
  // test if the data is valid (not yet implemented)
  _calData.acc = accel_readed;
  return 1;
}

int MPU9250::write_to_flash_accel_cal()
{
  Serial.println("write accel Calibration To Flash");
  EEPROM.put(ACC_CALIBRATION_ADDRESS, _calData.acc);
  EEPROM.commit();
  return 1;
}

int MPU9250::read_from_flash_mag_cal()
{
  mag_calib_data mag_readed;
  Serial.println("read mag Calibration From Flash");
  EEPROM.get(MAG_CALIBRATION_ADDRESS, mag_readed);
  // test if the data is valid (not yet implemented)
  _calData.mag = mag_readed;
  return 1;
}

int MPU9250::write_to_flash_mag_cal()
{
  Serial.println("write mag Calibration To Flash");
  EEPROM.put(MAG_CALIBRATION_ADDRESS, _calData.mag);
  EEPROM.commit();
  return 1;
}

int MPU9250::read_from_flash_all_cal()
{
  CalibrationData cal_readed;
  Serial.println("read all Calibration From Flash");
  EEPROM.get(GYRO_CALIBRATION_ADDRESS, cal_readed);
  // test if the data is valid (not yet implemented)
  _calData = cal_readed;
  return 1;
}

int MPU9250::write_to_flash_all_cal()
{
  Serial.println("write all Calibration To Flash");
  EEPROM.put(GYRO_CALIBRATION_ADDRESS, _calData);
  return 1;
}

/* sets the accelerometer full scale range to values other than default */
int MPU9250::setAccelRange(AccelRange range)
{
  // use low speed SPI for register setting
  _useSPIHS = false;
  switch (range)
  {
  case ACCEL_RANGE_2G:
  {
    // setting the accel range to 2G
    if (writeRegister(ACCEL_CONFIG, ACCEL_FS_SEL_2G) < 0)
    {
      return -1;
    }
    _accelScale = G * 2.0f / 32767.5f; // setting the accel scale to 2G
    break;
  }
  case ACCEL_RANGE_4G:
  {
    // setting the accel range to 4G
    if (writeRegister(ACCEL_CONFIG, ACCEL_FS_SEL_4G) < 0)
    {
      return -1;
    }
    _accelScale = G * 4.0f / 32767.5f; // setting the accel scale to 4G
    break;
  }
  case ACCEL_RANGE_8G:
  {
    // setting the accel range to 8G
    if (writeRegister(ACCEL_CONFIG, ACCEL_FS_SEL_8G) < 0)
    {
      return -1;
    }
    _accelScale = G * 8.0f / 32767.5f; // setting the accel scale to 8G
    break;
  }
  case ACCEL_RANGE_16G:
  {
    // setting the accel range to 16G
    if (writeRegister(ACCEL_CONFIG, ACCEL_FS_SEL_16G) < 0)
    {
      return -1;
    }
    _accelScale = G * 16.0f / 32767.5f; // setting the accel scale to 16G
    break;
  }
  }
  _accelRange = range;
  return 1;
}

/* sets the gyro full scale range to values other than default */
int MPU9250::setGyroRange(GyroRange range)
{
  // use low speed SPI for register setting
  _useSPIHS = false;
  switch (range)
  {
  case GYRO_RANGE_250DPS:
  {
    // setting the gyro range to 250DPS
    if (writeRegister(GYRO_CONFIG, GYRO_FS_SEL_250DPS) < 0)
    {
      return -1;
    }
    _gyroScale = 250.0f / 32767.5f * _d2r; // setting the gyro scale to 250DPS
    break;
  }
  case GYRO_RANGE_500DPS:
  {
    // setting the gyro range to 500DPS
    if (writeRegister(GYRO_CONFIG, GYRO_FS_SEL_500DPS) < 0)
    {
      return -1;
    }
    _gyroScale = 500.0f / 32767.5f * _d2r; // setting the gyro scale to 500DPS
    break;
  }
  case GYRO_RANGE_1000DPS:
  {
    // setting the gyro range to 1000DPS
    if (writeRegister(GYRO_CONFIG, GYRO_FS_SEL_1000DPS) < 0)
    {
      return -1;
    }
    _gyroScale = 1000.0f / 32767.5f * _d2r; // setting the gyro scale to 1000DPS
    break;
  }
  case GYRO_RANGE_2000DPS:
  {
    // setting the gyro range to 2000DPS
    if (writeRegister(GYRO_CONFIG, GYRO_FS_SEL_2000DPS) < 0)
    {
      return -1;
    }
    _gyroScale = 2000.0f / 32767.5f * _d2r; // setting the gyro scale to 2000DPS
    break;
  }
  }
  _gyroRange = range;
  return 1;
}

/* sets the DLPF bandwidth to values other than default */
int MPU9250::setDlpfBandwidth(DlpfBandwidth bandwidth)
{
  // use low speed SPI for register setting
  _useSPIHS = false;
  switch (bandwidth)
  {
  case DLPF_BANDWIDTH_184HZ:
  {
    if (writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_184) < 0)
    { // setting accel bandwidth to 184Hz
      return -1;
    }
    if (writeRegister(CONFIG, GYRO_DLPF_184) < 0)
    { // setting gyro bandwidth to 184Hz
      return -2;
    }
    break;
  }
  case DLPF_BANDWIDTH_92HZ:
  {
    if (writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_92) < 0)
    { // setting accel bandwidth to 92Hz
      return -1;
    }
    if (writeRegister(CONFIG, GYRO_DLPF_92) < 0)
    { // setting gyro bandwidth to 92Hz
      return -2;
    }
    break;
  }
  case DLPF_BANDWIDTH_41HZ:
  {
    if (writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_41) < 0)
    { // setting accel bandwidth to 41Hz
      return -1;
    }
    if (writeRegister(CONFIG, GYRO_DLPF_41) < 0)
    { // setting gyro bandwidth to 41Hz
      return -2;
    }
    break;
  }
  case DLPF_BANDWIDTH_20HZ:
  {
    if (writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_20) < 0)
    { // setting accel bandwidth to 20Hz
      return -1;
    }
    if (writeRegister(CONFIG, GYRO_DLPF_20) < 0)
    { // setting gyro bandwidth to 20Hz
      return -2;
    }
    break;
  }
  case DLPF_BANDWIDTH_10HZ:
  {
    if (writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_10) < 0)
    { // setting accel bandwidth to 10Hz
      return -1;
    }
    if (writeRegister(CONFIG, GYRO_DLPF_10) < 0)
    { // setting gyro bandwidth to 10Hz
      return -2;
    }
    break;
  }
  case DLPF_BANDWIDTH_5HZ:
  {
    if (writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_5) < 0)
    { // setting accel bandwidth to 5Hz
      return -1;
    }
    if (writeRegister(CONFIG, GYRO_DLPF_5) < 0)
    { // setting gyro bandwidth to 5Hz
      return -2;
    }
    break;
  }
  }
  _bandwidth = bandwidth;
  return 1;
}

/* sets the sample rate divider to values other than default */
int MPU9250::setSrd(uint8_t srd)
{
  // use low speed SPI for register setting
  _useSPIHS = false;
  /* setting the sample rate divider to 19 to facilitate setting up magnetometer */
  if (writeRegister(SMPDIV, 19) < 0)
  { // setting the sample rate divider
    return -1;
  }
  if (srd > 9)
  {
    // set AK8963 to Power Down
    if (writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN) < 0)
    {
      return -2;
    }
    delay(100); // long wait between AK8963 mode changes
    // set AK8963 to 16 bit resolution, 8 Hz update rate
    if (writeAK8963Register(AK8963_CNTL1, AK8963_CNT_MEAS1) < 0)
    {
      return -3;
    }
    delay(100); // long wait between AK8963 mode changes
    // instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
    readAK8963Registers(AK8963_HXL, 7, _buffer);
  }
  else
  {
    // set AK8963 to Power Down
    if (writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN) < 0)
    {
      return -2;
    }
    delay(100); // long wait between AK8963 mode changes
    // set AK8963 to 16 bit resolution, 100 Hz update rate
    if (writeAK8963Register(AK8963_CNTL1, AK8963_CNT_MEAS2) < 0)
    {
      return -3;
    }
    delay(100); // long wait between AK8963 mode changes
    // instruct the MPU9250 to get 7 bytes of data from the AK8963 at the sample rate
    readAK8963Registers(AK8963_HXL, 7, _buffer);
  }
  /* setting the sample rate divider */
  if (writeRegister(SMPDIV, srd) < 0)
  { // setting the sample rate divider
    return -4;
  }
  _srd = srd;
  return 1;
}

/* enables the data ready interrupt */
int MPU9250::enableDataReadyInterrupt()
{
  // use low speed SPI for register setting
  _useSPIHS = false;
  /* setting the interrupt */
  if (writeRegister(INT_PIN_CFG, INT_PULSE_50US) < 0)
  { // setup interrupt, 50 us pulse
    return -1;
  }
  if (writeRegister(INT_ENABLE, INT_RAW_RDY_EN) < 0)
  { // set to data ready
    return -2;
  }
  return 1;
}

/* disables the data ready interrupt */
int MPU9250::disableDataReadyInterrupt()
{
  // use low speed SPI for register setting
  _useSPIHS = false;
  if (writeRegister(INT_ENABLE, INT_DISABLE) < 0)
  { // disable interrupt
    return -1;
  }
  return 1;
}

/* configures and enables wake on motion, low power mode */
int MPU9250::enableWakeOnMotion(float womThresh_mg, LpAccelOdr odr)
{
  // use low speed SPI for register setting
  _useSPIHS = false;
  // set AK8963 to Power Down
  writeAK8963Register(AK8963_CNTL1, AK8963_PWR_DOWN);
  // reset the MPU9250
  writeRegister(PWR_MGMNT_1, PWR_RESET);
  // wait for MPU-9250 to come back up
  delay(1);
  if (writeRegister(PWR_MGMNT_1, 0x00) < 0)
  { // cycle 0, sleep 0, standby 0
    return -1;
  }
  if (writeRegister(PWR_MGMNT_2, DIS_GYRO) < 0)
  { // disable gyro measurements
    return -2;
  }
  if (writeRegister(ACCEL_CONFIG2, ACCEL_DLPF_184) < 0)
  { // setting accel bandwidth to 184Hz
    return -3;
  }
  if (writeRegister(INT_ENABLE, INT_WOM_EN) < 0)
  { // enabling interrupt to wake on motion
    return -4;
  }
  if (writeRegister(MOT_DETECT_CTRL, (ACCEL_INTEL_EN | ACCEL_INTEL_MODE)) < 0)
  { // enabling accel hardware intelligence
    return -5;
  }
  _womThreshold = map(womThresh_mg, 0, 1020, 0, 255);
  if (writeRegister(WOM_THR, _womThreshold) < 0)
  { // setting wake on motion threshold
    return -6;
  }
  if (writeRegister(LP_ACCEL_ODR, (uint8_t)odr) < 0)
  { // set frequency of wakeup
    return -7;
  }
  if (writeRegister(PWR_MGMNT_1, PWR_CYCLE) < 0)
  { // switch to accel low power mode
    return -8;
  }
  return 1;
}

/* configures and enables the FIFO buffer  */
int MPU9250FIFO::enableFifo(bool accel, bool gyro, bool mag, bool temp)
{
  // use low speed SPI for register setting
  _useSPIHS = false;
  if (writeRegister(USER_CTRL, (0x40 | I2C_MST_EN)) < 0)
  {
    return -1;
  }
  if (writeRegister(FIFO_EN, (accel * FIFO_ACCEL) | (gyro * FIFO_GYRO) | (mag * FIFO_MAG) | (temp * FIFO_TEMP)) < 0)
  {
    return -2;
  }
  _enFifoAccel = accel;
  _enFifoGyro = gyro;
  _enFifoMag = mag;
  _enFifoTemp = temp;
  _fifoFrameSize = accel * 6 + gyro * 6 + mag * 7 + temp * 2;
  return 1;
}

/* reads the most current data from MPU9250 and stores in buffer */
int MPU9250::readSensor()
{
  _useSPIHS = true; // use the high speed SPI for data readout
  // grab the data from the MPU9250
  if (readRegisters(ACCEL_OUT, 21, _buffer) < 0)
  {
    return -1;
  }
  // combine into 16 bit values
  _axcounts = (((int16_t)_buffer[0]) << 8) | _buffer[1];
  _aycounts = (((int16_t)_buffer[2]) << 8) | _buffer[3];
  _azcounts = (((int16_t)_buffer[4]) << 8) | _buffer[5];
  _tcounts = (((int16_t)_buffer[6]) << 8) | _buffer[7];
  _gxcounts = (((int16_t)_buffer[8]) << 8) | _buffer[9];
  _gycounts = (((int16_t)_buffer[10]) << 8) | _buffer[11];
  _gzcounts = (((int16_t)_buffer[12]) << 8) | _buffer[13];
  _hxcounts = (((int16_t)_buffer[15]) << 8) | _buffer[14];
  _hycounts = (((int16_t)_buffer[17]) << 8) | _buffer[16];
  _hzcounts = (((int16_t)_buffer[19]) << 8) | _buffer[18];
  // transform and convert to float values
  _ax = (((float)(tX[0] * _axcounts + tX[1] * _aycounts + tX[2] * _azcounts) * _accelScale) - _axb) * _axs;
  _ay = (((float)(tY[0] * _axcounts + tY[1] * _aycounts + tY[2] * _azcounts) * _accelScale) - _ayb) * _ays;
  _az = (((float)(tZ[0] * _axcounts + tZ[1] * _aycounts + tZ[2] * _azcounts) * _accelScale) - _azb) * _azs;
  _gx = ((float)(tX[0] * _gxcounts + tX[1] * _gycounts + tX[2] * _gzcounts) * _gyroScale) - _calData.gyr.Bias[0];
  _gy = ((float)(tY[0] * _gxcounts + tY[1] * _gycounts + tY[2] * _gzcounts) * _gyroScale) - _calData.gyr.Bias[1];
  _gz = ((float)(tZ[0] * _gxcounts + tZ[1] * _gycounts + tZ[2] * _gzcounts) * _gyroScale) - _calData.gyr.Bias[2];
  _hx = (((float)(_hxcounts)*_magScaleX) - _calData.mag.Bias[0]) * _calData.mag.RotationMatrix[0][0];
  _hy = (((float)(_hycounts)*_magScaleY) - _calData.mag.Bias[1]) * _calData.mag.RotationMatrix[1][1];
  _hz = (((float)(_hzcounts)*_magScaleZ) - _calData.mag.Bias[2]) * _calData.mag.RotationMatrix[2][2];
  _t = ((((float)_tcounts) - _tempOffset) / _tempScale) + _tempOffset; // _t : temperature in degrees C
  return 1;
}

/* returns the accelerometer measurement in the x direction, m/s/s */
float MPU9250::getAccelX_mss()
{
  return _ax;
}

/* returns the accelerometer measurement in the y direction, m/s/s */
float MPU9250::getAccelY_mss()
{
  return _ay;
}

/* returns the accelerometer measurement in the z direction, m/s/s */
float MPU9250::getAccelZ_mss()
{
  return _az;
}

/* returns the gyroscope measurement in the x direction, rad/s */
float MPU9250::getGyroX_rads()
{
  return _gx;
}

/* returns the gyroscope measurement in the y direction, rad/s */
float MPU9250::getGyroY_rads()
{
  return _gy;
}

/* returns the gyroscope measurement in the z direction, rad/s */
float MPU9250::getGyroZ_rads()
{
  return _gz;
}

/* returns the magnetometer measurement in the x direction, uT */
float MPU9250::getMagX_uT()
{
  return _hx;
}

/* returns the magnetometer measurement in the y direction, uT */
float MPU9250::getMagY_uT()
{
  return _hy;
}

/* returns the magnetometer measurement in the z direction, uT */
float MPU9250::getMagZ_uT()
{
  return _hz;
}

/* returns the die temperature, C */
float MPU9250::getTemperature_C()
{
  return _t;
}

/* reads data from the MPU9250 FIFO and stores in buffer */
int MPU9250FIFO::readFifo()
{
  _useSPIHS = true; // use the high speed SPI for data readout
  // get the fifo size
  readRegisters(FIFO_COUNT, 2, _buffer);
  _fifoSize = (((uint16_t)(_buffer[0] & 0x0F)) << 8) + (((uint16_t)_buffer[1]));
  // read and parse the buffer
  for (size_t i = 0; i < _fifoSize / _fifoFrameSize; i++)
  {
    // grab the data from the MPU9250
    if (readRegisters(FIFO_READ, _fifoFrameSize, _buffer) < 0)
    {
      return -1;
    }
    if (_enFifoAccel)
    {
      // combine into 16 bit values
      _axcounts = (((int16_t)_buffer[0]) << 8) | _buffer[1];
      _aycounts = (((int16_t)_buffer[2]) << 8) | _buffer[3];
      _azcounts = (((int16_t)_buffer[4]) << 8) | _buffer[5];
      // transform and convert to float values
      _axFifo[i] = (((float)(tX[0] * _axcounts + tX[1] * _aycounts + tX[2] * _azcounts) * _accelScale) - _axb) * _axs;
      _ayFifo[i] = (((float)(tY[0] * _axcounts + tY[1] * _aycounts + tY[2] * _azcounts) * _accelScale) - _ayb) * _ays;
      _azFifo[i] = (((float)(tZ[0] * _axcounts + tZ[1] * _aycounts + tZ[2] * _azcounts) * _accelScale) - _azb) * _azs;
      _aSize = _fifoSize / _fifoFrameSize;
    }
    if (_enFifoTemp)
    {
      // combine into 16 bit values
      _tcounts = (((int16_t)_buffer[0 + _enFifoAccel * 6]) << 8) | _buffer[1 + _enFifoAccel * 6];
      // transform and convert to float values
      _tFifo[i] = ((((float)_tcounts) - _tempOffset) / _tempScale) + _tempOffset;
      _tSize = _fifoSize / _fifoFrameSize;
    }
    if (_enFifoGyro)
    {
      // combine into 16 bit values
      _gxcounts = (((int16_t)_buffer[0 + _enFifoAccel * 6 + _enFifoTemp * 2]) << 8) | _buffer[1 + _enFifoAccel * 6 + _enFifoTemp * 2];
      _gycounts = (((int16_t)_buffer[2 + _enFifoAccel * 6 + _enFifoTemp * 2]) << 8) | _buffer[3 + _enFifoAccel * 6 + _enFifoTemp * 2];
      _gzcounts = (((int16_t)_buffer[4 + _enFifoAccel * 6 + _enFifoTemp * 2]) << 8) | _buffer[5 + _enFifoAccel * 6 + _enFifoTemp * 2];
      // transform and convert to float values
      _gxFifo[i] = ((float)(tX[0] * _gxcounts + tX[1] * _gycounts + tX[2] * _gzcounts) * _gyroScale) - _calData.gyr.Bias[0];
      _gyFifo[i] = ((float)(tY[0] * _gxcounts + tY[1] * _gycounts + tY[2] * _gzcounts) * _gyroScale) - _calData.gyr.Bias[1];
      _gzFifo[i] = ((float)(tZ[0] * _gxcounts + tZ[1] * _gycounts + tZ[2] * _gzcounts) * _gyroScale) - _calData.gyr.Bias[2];
      _gSize = _fifoSize / _fifoFrameSize;
    }
    if (_enFifoMag)
    {
      // combine into 16 bit values
      _hxcounts = (((int16_t)_buffer[1 + _enFifoAccel * 6 + _enFifoTemp * 2 + _enFifoGyro * 6]) << 8) | _buffer[0 + _enFifoAccel * 6 + _enFifoTemp * 2 + _enFifoGyro * 6];
      _hycounts = (((int16_t)_buffer[3 + _enFifoAccel * 6 + _enFifoTemp * 2 + _enFifoGyro * 6]) << 8) | _buffer[2 + _enFifoAccel * 6 + _enFifoTemp * 2 + _enFifoGyro * 6];
      _hzcounts = (((int16_t)_buffer[5 + _enFifoAccel * 6 + _enFifoTemp * 2 + _enFifoGyro * 6]) << 8) | _buffer[4 + _enFifoAccel * 6 + _enFifoTemp * 2 + _enFifoGyro * 6];
      // transform and convert to float values
      _hxFifo[i] = (((float)(_hxcounts)*_magScaleX) - _calData.mag.Bias[0]) * _calData.mag.RotationMatrix[0][0];
      _hyFifo[i] = (((float)(_hycounts)*_magScaleY) - _calData.mag.Bias[1]) * _calData.mag.RotationMatrix[1][1];
      _hzFifo[i] = (((float)(_hzcounts)*_magScaleZ) - _calData.mag.Bias[2]) * _calData.mag.RotationMatrix[2][2];
      _hSize = _fifoSize / _fifoFrameSize;
    }
  }
  return 1;
}

/* returns the accelerometer FIFO size and data in the x direction, m/s/s */
void MPU9250FIFO::getFifoAccelX_mss(size_t *size, float *data)
{
  *size = _aSize;
  memcpy(data, _axFifo, _aSize * sizeof(float));
}

/* returns the accelerometer FIFO size and data in the y direction, m/s/s */
void MPU9250FIFO::getFifoAccelY_mss(size_t *size, float *data)
{
  *size = _aSize;
  memcpy(data, _ayFifo, _aSize * sizeof(float));
}

/* returns the accelerometer FIFO size and data in the z direction, m/s/s */
void MPU9250FIFO::getFifoAccelZ_mss(size_t *size, float *data)
{
  *size = _aSize;
  memcpy(data, _azFifo, _aSize * sizeof(float));
}

/* returns the gyroscope FIFO size and data in the x direction, rad/s */
void MPU9250FIFO::getFifoGyroX_rads(size_t *size, float *data)
{
  *size = _gSize;
  memcpy(data, _gxFifo, _gSize * sizeof(float));
}

/* returns the gyroscope FIFO size and data in the y direction, rad/s */
void MPU9250FIFO::getFifoGyroY_rads(size_t *size, float *data)
{
  *size = _gSize;
  memcpy(data, _gyFifo, _gSize * sizeof(float));
}

/* returns the gyroscope FIFO size and data in the z direction, rad/s */
void MPU9250FIFO::getFifoGyroZ_rads(size_t *size, float *data)
{
  *size = _gSize;
  memcpy(data, _gzFifo, _gSize * sizeof(float));
}

/* returns the magnetometer FIFO size and data in the x direction, uT */
void MPU9250FIFO::getFifoMagX_uT(size_t *size, float *data)
{
  *size = _hSize;
  memcpy(data, _hxFifo, _hSize * sizeof(float));
}

/* returns the magnetometer FIFO size and data in the y direction, uT */
void MPU9250FIFO::getFifoMagY_uT(size_t *size, float *data)
{
  *size = _hSize;
  memcpy(data, _hyFifo, _hSize * sizeof(float));
}

/* returns the magnetometer FIFO size and data in the z direction, uT */
void MPU9250FIFO::getFifoMagZ_uT(size_t *size, float *data)
{
  *size = _hSize;
  memcpy(data, _hzFifo, _hSize * sizeof(float));
}

/* returns the die temperature FIFO size and data, C */
void MPU9250FIFO::getFifoTemperature_C(size_t *size, float *data)
{
  *size = _tSize;
  memcpy(data, _tFifo, _tSize * sizeof(float));
}

/* estimates the gyro biases */
int MPU9250::calibrateGyro()
{
  // set the range, bandwidth, and srd
  if (setGyroRange(GYRO_RANGE_250DPS) < 0)
  {
    return -1;
  }
  if (setDlpfBandwidth(DLPF_BANDWIDTH_20HZ) < 0)
  {
    return -2;
  }
  if (setSrd(19) < 0)
  {
    return -3;
  }

  // take samples and find bias
  _gxbD = 0;
  _gybD = 0;
  _gzbD = 0;
  for (size_t i = 0; i < _numSamples; i++)
  {
    readSensor();
    _gxbD += (getGyroX_rads() + _calData.gyr.Bias[0]) / ((double)_numSamples);
    _gybD += (getGyroY_rads() + _calData.gyr.Bias[1]) / ((double)_numSamples);
    _gzbD += (getGyroZ_rads() + _calData.gyr.Bias[2]) / ((double)_numSamples);
    delay(20);
  }
  _calData.gyr.Bias[0] = (float)_gxbD;
  _calData.gyr.Bias[1] = (float)_gybD;
  _calData.gyr.Bias[2] = (float)_gzbD;

  // set the range, bandwidth, and srd back to what they were
  if (setGyroRange(_gyroRange) < 0)
  {
    return -4;
  }
  if (setDlpfBandwidth(_bandwidth) < 0)
  {
    return -5;
  }
  if (setSrd(_srd) < 0)
  {
    return -6;
  }
  return 1;
}

int MPU9250::calibrateGyro_advanced(const unsigned int numberOfSamples, const unsigned int dt_ms, const bool write_to_flash)
{

  Vector g = Vector(3);
  gyroCalibration.reset();
  for (unsigned int i = 0; i < numberOfSamples; i++)
  {
    readSensor();

    g.set(0, (double)getGyroX_rads());
    g.set(1, (double)getGyroY_rads());
    g.set(2, (double)getGyroZ_rads());
    gyroCalibration.process(g);
    delay(dt_ms);
  }
  _calData.gyr.Bias[0] = (float)gyroCalibration.bias(0);
  _calData.gyr.Bias[1] = (float)gyroCalibration.bias(1);
  _calData.gyr.Bias[2] = (float)gyroCalibration.bias(2);
  _calData.gyr.std_dev = (float)gyroCalibration.stdDev.norm();
  Serial.print("_gxb: ");
  Serial.println(_calData.gyr.Bias[0]);
  Serial.print("_gyb: ");
  Serial.println(_calData.gyr.Bias[1]);
  Serial.print("_gzb: ");
  Serial.println(_calData.gyr.Bias[2]);
  Serial.print("_g_std_dev: ");
  Serial.println(_calData.gyr.std_dev);

  if (write_to_flash)
  {
    write_to_flash_gyro_cal();
  }
  return 1;
}

/* returns the gyro bias in the X direction, rad/s */
float MPU9250::getGyroBiasX_rads()
{
  return _calData.gyr.Bias[0];
}

/* returns the gyro bias in the Y direction, rad/s */
float MPU9250::getGyroBiasY_rads()
{
  return _calData.gyr.Bias[1];
}

/* returns the gyro bias in the Z direction, rad/s */
float MPU9250::getGyroBiasZ_rads()
{
  return _calData.gyr.Bias[2];
}

/* sets the gyro bias in the X direction to bias, rad/s */
void MPU9250::setGyroBiasX_rads(float bias)
{
  _calData.gyr.Bias[0] = bias;
}

/* sets the gyro bias in the Y direction to bias, rad/s */
void MPU9250::setGyroBiasY_rads(float bias)
{
  _calData.gyr.Bias[1] = bias;
}

/* sets the gyro bias in the Z direction to bias, rad/s */
void MPU9250::setGyroBiasZ_rads(float bias)
{
  _calData.gyr.Bias[2] = bias;
}

/* finds bias and scale factor calibration for the accelerometer,
this should be run for each axis in each direction (6 total) to find
the min and max values along each */
int MPU9250::calibrateAccel()
{
  // set the range, bandwidth, and srd
  if (setAccelRange(ACCEL_RANGE_2G) < 0)
  {
    return -1;
  }
  if (setDlpfBandwidth(DLPF_BANDWIDTH_20HZ) < 0)
  {
    return -2;
  }
  if (setSrd(19) < 0)
  {
    return -3;
  }

  // take samples and find min / max
  _axbD = 0;
  _aybD = 0;
  _azbD = 0;
  for (size_t i = 0; i < _numSamples; i++)
  {
    readSensor();
    _axbD += (getAccelX_mss() / _axs + _axb) / ((double)_numSamples);
    _aybD += (getAccelY_mss() / _ays + _ayb) / ((double)_numSamples);
    _azbD += (getAccelZ_mss() / _azs + _azb) / ((double)_numSamples);
    delay(20);
  }
  if (_axbD > 9.0f)
  {
    _axmax = (float)_axbD;
  }
  if (_aybD > 9.0f)
  {
    _aymax = (float)_aybD;
  }
  if (_azbD > 9.0f)
  {
    _azmax = (float)_azbD;
  }
  if (_axbD < -9.0f)
  {
    _axmin = (float)_axbD;
  }
  if (_aybD < -9.0f)
  {
    _aymin = (float)_aybD;
  }
  if (_azbD < -9.0f)
  {
    _azmin = (float)_azbD;
  }

  // find bias and scale factor
  if ((abs(_axmin) > 9.0f) && (abs(_axmax) > 9.0f))
  {
    _axb = (_axmin + _axmax) / 2.0f;
    _axs = G / ((abs(_axmin) + abs(_axmax)) / 2.0f);
  }
  if ((abs(_aymin) > 9.0f) && (abs(_aymax) > 9.0f))
  {
    _ayb = (_aymin + _aymax) / 2.0f;
    _ays = G / ((abs(_aymin) + abs(_aymax)) / 2.0f);
  }
  if ((abs(_azmin) > 9.0f) && (abs(_azmax) > 9.0f))
  {
    _azb = (_azmin + _azmax) / 2.0f;
    _azs = G / ((abs(_azmin) + abs(_azmax)) / 2.0f);
  }

  // set the range, bandwidth, and srd back to what they were
  if (setAccelRange(_accelRange) < 0)
  {
    return -4;
  }
  if (setDlpfBandwidth(_bandwidth) < 0)
  {
    return -5;
  }
  if (setSrd(_srd) < 0)
  {
    return -6;
  }
  return 1;
}

bool MPU9250::is_moving()
{
  float gyr_sum = abs(getGyroX_rads()) + abs(getGyroY_rads()) + abs(getGyroZ_rads());
  if (gyr_sum > gyro_threshold)
  {
    return true;
  }

  return false;
}
int MPU9250::calibrateAccel_advanced(const unsigned int numberOfSamples, const bool write_to_flash)
{
  // collect data
  unsigned int sampleCounter = 0;
  unsigned long t0 = millis();
  double sample[numberOfSamples * 3];
  while (sampleCounter < numberOfSamples)
  {
    readSensor();
    if (!is_moving())
    {
      double distance = 10;
      if (sampleCounter > 0)
        distance = abs(sample[(sampleCounter - 1) * 3] - (double)getAccelX_mss()) + abs(sample[(sampleCounter - 1) * 3 + 1] - (double)getAccelY_mss()) + abs(sample[(sampleCounter - 1) * 3 + 2] - (double)getAccelZ_mss());
      if (distance > 2 and millis() - t0 >= 500)
      {
        sample[sampleCounter * 3] = getAccelX_mss();
        sample[sampleCounter * 3 + 1] = getAccelY_mss();
        sample[sampleCounter * 3 + 2] = getAccelZ_mss();
        sampleCounter++;
        Serial.println("waitting for sample " + String(sampleCounter) + "/" + String(numberOfSamples));
        t0 = millis();
      }
    }
    else
    {
      t0 = millis();
    }
    delay(20);
  }
  Serial.println("start computing...");
  accCalibration.calibrate(sample, sampleCounter, 0.05);
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      _calData.acc.RotationMatrix[i][j] = (float)accCalibration.ellipsoidFitting.A(i, j);
    }
    _calData.acc.Bias[i] = (float)accCalibration.ellipsoidFitting.B(i);
  }
  _calData.acc.std_dev = sqrt(accCalibration.ellipsoidFitting.minimum_cost);

  Serial.println(accCalibration.ellipsoidFitting.A.to_str());
  Serial.println(accCalibration.ellipsoidFitting.B.to_str());
  Serial.println(sqrt(accCalibration.ellipsoidFitting.minimum_cost));

  if (write_to_flash)
  {
    write_to_flash_accel_cal();
  }
  return 1;
}

/* returns the accelerometer bias in the X direction, m/s/s */
float MPU9250::getAccelBiasX_mss()
{
  return _axb;
}

/* returns the accelerometer scale factor in the X direction */
float MPU9250::getAccelScaleFactorX()
{
  return _axs;
}

/* returns the accelerometer bias in the Y direction, m/s/s */
float MPU9250::getAccelBiasY_mss()
{
  return _ayb;
}

/* returns the accelerometer scale factor in the Y direction */
float MPU9250::getAccelScaleFactorY()
{
  return _ays;
}

/* returns the accelerometer bias in the Z direction, m/s/s */
float MPU9250::getAccelBiasZ_mss()
{
  return _azb;
}

/* returns the accelerometer scale factor in the Z direction */
float MPU9250::getAccelScaleFactorZ()
{
  return _azs;
}

/* sets the accelerometer bias (m/s/s) and scale factor in the X direction */
void MPU9250::setAccelCalX(float bias, float scaleFactor)
{
  _axb = bias;
  _axs = scaleFactor;
}

/* sets the accelerometer bias (m/s/s) and scale factor in the Y direction */
void MPU9250::setAccelCalY(float bias, float scaleFactor)
{
  _ayb = bias;
  _ays = scaleFactor;
}

/* sets the accelerometer bias (m/s/s) and scale factor in the Z direction */
void MPU9250::setAccelCalZ(float bias, float scaleFactor)
{
  _azb = bias;
  _azs = scaleFactor;
}

/* finds bias and scale factor calibration for the magnetometer,
the sensor should be rotated in a figure 8 motion until complete */
int MPU9250::calibrateMag()
{
  // set the srd
  if (setSrd(19) < 0)
  {
    return -1;
  }

  // get a starting set of data
  readSensor();
  _hxmax = getMagX_uT();
  _hxmin = getMagX_uT();
  _hymax = getMagY_uT();
  _hymin = getMagY_uT();
  _hzmax = getMagZ_uT();
  _hzmin = getMagZ_uT();

  // collect data to find max / min in each channel
  _counter = 0;
  while (_counter < _maxCounts)
  {
    _delta = 0.0f;
    _framedelta = 0.0f;
    readSensor();
    _hxfilt = (_hxfilt * ((float)_coeff - 1) + (getMagX_uT() / _calData.mag.RotationMatrix[0][0] + _calData.mag.Bias[0])) / ((float)_coeff);
    _hyfilt = (_hyfilt * ((float)_coeff - 1) + (getMagY_uT() / _calData.mag.RotationMatrix[1][1] + _calData.mag.Bias[1])) / ((float)_coeff);
    _hzfilt = (_hzfilt * ((float)_coeff - 1) + (getMagZ_uT() / _calData.mag.RotationMatrix[2][2] + _calData.mag.Bias[2])) / ((float)_coeff);
    if (_hxfilt > _hxmax)
    {
      _delta = _hxfilt - _hxmax;
      _hxmax = _hxfilt;
    }
    if (_delta > _framedelta)
    {
      _framedelta = _delta;
    }
    if (_hyfilt > _hymax)
    {
      _delta = _hyfilt - _hymax;
      _hymax = _hyfilt;
    }
    if (_delta > _framedelta)
    {
      _framedelta = _delta;
    }
    if (_hzfilt > _hzmax)
    {
      _delta = _hzfilt - _hzmax;
      _hzmax = _hzfilt;
    }
    if (_delta > _framedelta)
    {
      _framedelta = _delta;
    }
    if (_hxfilt < _hxmin)
    {
      _delta = abs(_hxfilt - _hxmin);
      _hxmin = _hxfilt;
    }
    if (_delta > _framedelta)
    {
      _framedelta = _delta;
    }
    if (_hyfilt < _hymin)
    {
      _delta = abs(_hyfilt - _hymin);
      _hymin = _hyfilt;
    }
    if (_delta > _framedelta)
    {
      _framedelta = _delta;
    }
    if (_hzfilt < _hzmin)
    {
      _delta = abs(_hzfilt - _hzmin);
      _hzmin = _hzfilt;
    }
    if (_delta > _framedelta)
    {
      _framedelta = _delta;
    }
    if (_framedelta > _deltaThresh)
    {
      _counter = 0;
    }
    else
    {
      _counter++;
    }
    delay(20);
  }

  // find the magnetometer bias
  _calData.mag.Bias[0] = (_hxmax + _hxmin) / 2.0f;
  _calData.mag.Bias[1] = (_hymax + _hymin) / 2.0f;
  _calData.mag.Bias[2] = (_hzmax + _hzmin) / 2.0f;

  // find the magnetometer scale factor
  _calData.mag.RotationMatrix[0][0] = (_hxmax - _hxmin) / 2.0f;
  _calData.mag.RotationMatrix[1][1] = (_hymax - _hymin) / 2.0f;
  _calData.mag.RotationMatrix[2][2] = (_hzmax - _hzmin) / 2.0f;
  const float _avgs = (_calData.mag.RotationMatrix[0][0] + _calData.mag.RotationMatrix[1][1] + _calData.mag.RotationMatrix[2][2]) / 3.0f;
  _calData.mag.RotationMatrix[0][0] = _avgs / _calData.mag.RotationMatrix[0][0];
  _calData.mag.RotationMatrix[1][1] = _avgs / _calData.mag.RotationMatrix[1][1];
  _calData.mag.RotationMatrix[2][2] = _avgs / _calData.mag.RotationMatrix[2][2];

  // set the srd back to what it was
  if (setSrd(_srd) < 0)
  {
    return -2;
  }
  return 1;
}

int MPU9250::calibrateMag_advanced(const unsigned int numberOfSamples, const bool write_to_flash)
{
  // collect data
  unsigned int sampleCounter = 0;
  double sample[numberOfSamples * 3];
  while (sampleCounter < numberOfSamples)
  {
;
    double distance = 10;
    if (sampleCounter > 0)
      distance = abs(sample[(sampleCounter - 1) * 3] - (double)getMagX_uT()) + abs(sample[(sampleCounter - 1) * 3 + 1] - (double)getMagY_uT()) + abs(sample[(sampleCounter - 1) * 3 + 2] - (double)getMagZ_uT());
    if (distance > 5)
    {
      sample[sampleCounter * 3] = getMagX_uT();
      sample[sampleCounter * 3 + 1] = getMagY_uT();
      sample[sampleCounter * 3 + 2] = getMagZ_uT();
      sampleCounter++;
      Serial.println("waitting for sample " + String(sampleCounter) + "/" + String(numberOfSamples));
    }
    delay(200);
  }
  Serial.println("start computing...");
  magCalibration.calibrate(sample, sampleCounter, 1);
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      _calData.mag.RotationMatrix[i][j] = (float)magCalibration.ellipsoidFitting.A(i, j);
    }
    _calData.mag.Bias[i] = (float)magCalibration.ellipsoidFitting.B(i);
  }
  _calData.mag.std_dev = sqrt(magCalibration.ellipsoidFitting.minimum_cost);


  if (write_to_flash)
  {
    write_to_flash_mag_cal();
  }
  return 0;
}

String MPU9250::mag_calibration_advanced_step(const unsigned int numberOfSamples, float minimum_distance, bool write_to_flash)
{
  if (mag_advance_cal_counter == 0)
  {
    if (readSensor()<0) return "";// error reading sensor data;
    // initialise data
    _hxmin = getMagX_uT();
    _hxmax = getMagX_uT();
    _hymin = getMagY_uT();
    _hymax = getMagY_uT();
    _hzmin = getMagZ_uT();
    _hzmax = getMagZ_uT();
    _mag_samples = new double[numberOfSamples * 3];
    mag_advance_cal_counter++;
    return "begin of advanced mag calibration";
  }
  else if (mag_advance_cal_counter <= numberOfSamples)
  {
    // collect data
    if (readSensor()<0) return "";// error reading sensor data;

    double distance = minimum_distance * 2;
    if (mag_advance_cal_counter > 1)
      distance = abs(_mag_samples[(mag_advance_cal_counter - 2) * 3] - (double)getMagX_uT()) + abs(_mag_samples[(mag_advance_cal_counter - 2) * 3 + 1] - (double)getMagY_uT()) + abs(_mag_samples[(mag_advance_cal_counter - 2) * 3 + 2] - (double)getMagZ_uT());
    if (distance > minimum_distance) // if the vector is different from the previous one by more than 5uT
    {
      _mag_samples[(mag_advance_cal_counter - 1) * 3] = getMagX_uT();
      _mag_samples[(mag_advance_cal_counter - 1) * 3 + 1] = getMagY_uT();
      _mag_samples[(mag_advance_cal_counter - 1) * 3 + 2] = getMagZ_uT();
      if (getMagX_uT() < _hxmin) _hxmin = getMagX_uT();
      if (getMagX_uT() > _hxmax) _hxmax = getMagX_uT();
      if (getMagY_uT() < _hymin) _hymin = getMagY_uT();
      if (getMagY_uT() > _hymax) _hymax = getMagY_uT();
      if (getMagZ_uT() < _hzmin) _hzmin = getMagZ_uT();
      if (getMagZ_uT() > _hzmax) _hzmax = getMagZ_uT();
  
      mag_advance_cal_counter++;

      //find min and max
      if (mag_advance_cal_counter == 2)
      {
        
      }

      if (mag_advance_cal_counter <= numberOfSamples)
        return "waitting for sample " + String(mag_advance_cal_counter) + "/" + String(numberOfSamples) + " distance=" + String(distance);
    }
    return "";
  }
  else // mag_advance_cal_counter > numberOfSamples
  {
    // compute calibration
    double *initial_params = new double[12];
    initial_params[0] = (_hxmax + _hxmin) / 2;
    initial_params[1] = 0;
    initial_params[2] = 0;
    initial_params[3] = (_hymax + _hymin) / 2;
    initial_params[4] = 0;
    initial_params[5] = 0;
    initial_params[6] = (_hzmax + _hzmin) / 2;
    initial_params[7] = 0;
    initial_params[8] = 0;
    initial_params[9] = (_hxmax + _hxmin) / 2;
    initial_params[10] = (_hymax + _hymin) / 2;
    initial_params[11] = (_hzmax + _hzmin) / 2;

    magCalibration.calibrate(_mag_samples, numberOfSamples, 0.015, initial_params);
    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        _calData.mag.RotationMatrix[i][j] = (float)magCalibration.ellipsoidFitting.A(i, j);
      }
      _calData.mag.Bias[i] = (float)magCalibration.ellipsoidFitting.B(i);
    }
    _calData.mag.std_dev = sqrt(magCalibration.ellipsoidFitting.minimum_cost);

    // clear data
    mag_advance_cal_counter = 0;
    delete[] _mag_samples;

    if (write_to_flash)
      write_to_flash_mag_cal();
    return "COMPLETED";
  }
}
/* returns the magnetometer bias in the X direction, uT */
float MPU9250::getMagBiasX_uT()
{
  return _calData.mag.Bias[0];
}

/* returns the magnetometer scale factor in the X direction */
float MPU9250::getMagScaleFactorX()
{
  return _calData.mag.RotationMatrix[0][0];
}

/* returns the magnetometer bias in the Y direction, uT */
float MPU9250::getMagBiasY_uT()
{
  return _calData.mag.Bias[1];
}

/* returns the magnetometer scale factor in the Y direction */
float MPU9250::getMagScaleFactorY()
{
  return _calData.mag.RotationMatrix[1][1];
}

/* returns the magnetometer bias in the Z direction, uT */
float MPU9250::getMagBiasZ_uT()
{
  return _calData.mag.Bias[2];
}

/* returns the magnetometer scale factor in the Z direction */
float MPU9250::getMagScaleFactorZ()
{
  return _calData.mag.RotationMatrix[2][2];
}

/* sets the magnetometer bias (uT) and scale factor in the X direction */
void MPU9250::setMagCalX(float bias, float scaleFactor)
{
  _calData.mag.Bias[0] = bias;
  _calData.mag.RotationMatrix[0][0] = scaleFactor;
}

/* sets the magnetometer bias (uT) and scale factor in the Y direction */
void MPU9250::setMagCalY(float bias, float scaleFactor)
{
  _calData.mag.Bias[1] = bias;
  _calData.mag.RotationMatrix[1][1] = scaleFactor;
}

/* sets the magnetometer bias (uT) and scale factor in the Z direction */
void MPU9250::setMagCalZ(float bias, float scaleFactor)
{
  _calData.mag.Bias[2] = bias;
  _calData.mag.RotationMatrix[2][2] = scaleFactor;
}

/* writes a byte to MPU9250 register given a register address and data */
int MPU9250::writeRegister(uint8_t subAddress, uint8_t data)
{
  /* write data to device */
  if (_useSPI)
  {
    _spi->beginTransaction(SPISettings(SPI_LS_CLOCK, MSBFIRST, SPI_MODE3)); // begin the transaction
    digitalWrite(_csPin, LOW);                                              // select the MPU9250 chip
    _spi->transfer(subAddress);                                             // write the register address
    _spi->transfer(data);                                                   // write the data
    digitalWrite(_csPin, HIGH);                                             // deselect the MPU9250 chip
    _spi->endTransaction();                                                 // end the transaction
  }
  else
  {
    _i2c->beginTransmission(_address); // open the device
    _i2c->write(subAddress);           // write the register address
    _i2c->write(data);                 // write the data
    _i2c->endTransmission();
  }

  delay(10);

  /* read back the register */
  readRegisters(subAddress, 1, _buffer);
  /* check the read back register against the written register */
  if (_buffer[0] == data)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

/* reads registers from MPU9250 given a starting register address, number of bytes, and a pointer to store data */
int MPU9250::readRegisters(uint8_t subAddress, uint8_t count, uint8_t *dest)
{
  if (_useSPI)
  {
    // begin the transaction
    if (_useSPIHS)
    {
      _spi->beginTransaction(SPISettings(SPI_HS_CLOCK, MSBFIRST, SPI_MODE3));
    }
    else
    {
      _spi->beginTransaction(SPISettings(SPI_LS_CLOCK, MSBFIRST, SPI_MODE3));
    }
    digitalWrite(_csPin, LOW);             // select the MPU9250 chip
    _spi->transfer(subAddress | SPI_READ); // specify the starting register address
    for (uint8_t i = 0; i < count; i++)
    {
      dest[i] = _spi->transfer(0x00); // read the data
    }
    digitalWrite(_csPin, HIGH); // deselect the MPU9250 chip
    _spi->endTransaction();     // end the transaction
    return 1;
  }
  else
  {
    _i2c->beginTransmission(_address); // open the device
    _i2c->write(subAddress);           // specify the starting register address
    _i2c->endTransmission(false);
    _numBytes = _i2c->requestFrom(_address, count); // specify the number of bytes to receive
    if (_numBytes == count)
    {
      for (uint8_t i = 0; i < count; i++)
      {
        dest[i] = _i2c->read();
      }
      return 1;
    }
    else
    {
      return -1;
    }
  }
}

/* writes a register to the AK8963 given a register address and data */
int MPU9250::writeAK8963Register(uint8_t subAddress, uint8_t data)
{
  // set slave 0 to the AK8963 and set for write
  if (writeRegister(I2C_SLV0_ADDR, AK8963_I2C_ADDR) < 0)
  {
    return -1;
  }
  // set the register to the desired AK8963 sub address
  if (writeRegister(I2C_SLV0_REG, subAddress) < 0)
  {
    return -2;
  }
  // store the data for write
  if (writeRegister(I2C_SLV0_DO, data) < 0)
  {
    return -3;
  }
  // enable I2C and send 1 byte
  if (writeRegister(I2C_SLV0_CTRL, I2C_SLV0_EN | (uint8_t)1) < 0)
  {
    return -4;
  }
  // read the register and confirm
  if (readAK8963Registers(subAddress, 1, _buffer) < 0)
  {
    return -5;
  }
  if (_buffer[0] == data)
  {
    return 1;
  }
  else
  {
    return -6;
  }
}

/* reads registers from the AK8963 */
int MPU9250::readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t *dest)
{
  // set slave 0 to the AK8963 and set for read
  if (writeRegister(I2C_SLV0_ADDR, AK8963_I2C_ADDR | I2C_READ_FLAG) < 0)
  {
    return -1;
  }
  // set the register to the desired AK8963 sub address
  if (writeRegister(I2C_SLV0_REG, subAddress) < 0)
  {
    return -2;
  }
  // enable I2C and request the bytes
  if (writeRegister(I2C_SLV0_CTRL, I2C_SLV0_EN | count) < 0)
  {
    return -3;
  }
  delay(1); // takes some time for these registers to fill
            // read the bytes off the MPU9250 EXT_SENS_DATA registers
  _status = readRegisters(EXT_SENS_DATA_00, count, dest);
  return _status;
}

/* gets the MPU9250 WHO_AM_I register value, expected to be 0x71 */
int MPU9250::whoAmI()
{
  // read the WHO AM I register
  if (readRegisters(WHO_AM_I, 1, _buffer) < 0)
  {
    return -1;
  }
  // return the register value
  return _buffer[0];
}

/* gets the AK8963 WHO_AM_I register value, expected to be 0x48 */
int MPU9250::whoAmIAK8963()
{
  // read the WHO AM I register
  if (readAK8963Registers(AK8963_WHO_AM_I, 1, _buffer) < 0)
  {
    return -1;
  }
  // return the register value
  return _buffer[0];
}
