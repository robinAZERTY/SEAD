    std::cout << ellipsoid.A.to_str() << std::endl;
    std::cout << ellipsoid.B.to_str() << std::endl;
    std::cout << "minimum cost = " << ellipsoid.minimum_cost << std::endl;
    std::cout << "average distance = " << sqrt(ellipsoid.minimum_cost) << std::endl;