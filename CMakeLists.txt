cmake_minimum_required(VERSION 3.5)

# Название проекта
project(fullstack-test-proj)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Указываем файлы исходного кода
set(SOURCES
    main.cpp
    AccountController.cpp
    BankAccount.cpp
)

# Указываем заголовочные файлы для IDE (не обязательно для сборки)
set(HEADERS
    AccountController.h
    BankAccount.h
    FrontendController.h
)

# Подключаем фреймворк Drogon
find_package(Drogon CONFIG REQUIRED)

# Создаем исполняемый файл из исходных файлов
add_executable(BankApp ${SOURCES} ${HEADERS})

# Линкуем фреймворк Drogon к исполняемому файлу
target_link_libraries(BankApp PRIVATE Drogon::Drogon)

# Создаем тестовый исполняемый файл, добавляем исходники и заголовки, необходимые для тестов
add_executable(BankAppTests BankAccount_test.cpp BankAccount.cpp)

# Линкуем необходимые библиотеки к тестам
target_link_libraries(BankAppTests PRIVATE Drogon::Drogon)

# Добавляем возможность запуска тестов
enable_testing()
add_test(NAME BankAppTests COMMAND BankAppTests)
