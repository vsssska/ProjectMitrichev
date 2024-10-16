# Указываем базовый образ с Drogon
FROM drogonframework/drogon

# Устанавливаем зависимости для сборки
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# Копируем код в контейнер
WORKDIR /app
COPY . .

# Собираем проект с помощью CMake
RUN mkdir build && cd build && cmake .. && make

EXPOSE 8848

# Запускаем сервер при запуске контейнера
CMD ["./build/BankApp"]
