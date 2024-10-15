# Указываем базовый образ с Drogon
FROM drogonframework/drogon

# Копируем код в контейнер
WORKDIR /app
COPY . .

# Собираем проект с помощью CMake
RUN mkdir build && cd build && cmake .. && make

# Запускаем сервер при запуске контейнера
CMD ["./build/BankApp"]
