# JimsBarCashingApp

JimsBarCashingApp is a Qt-based application designed to manage bar operations, including handling cocktails, managing a database, and providing a user-friendly interface.

## Features
- **Cocktail Management**: Manage cocktail recipes and details.
- **Database Integration**: Uses SQLite for storing and retrieving data.
- **QML Interface**: Provides a modern and responsive user interface using QML.

## Prerequisites
- **CMake**: Version 3.10 or higher.
- **Qt5**: Ensure the following Qt5 components are installed:
  - Core
  - Sql
  - Widgets
  - Qml

## Building the Project
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd JimsBarCashingApp
   ```
2. Create a build directory and navigate to it:
   ```bash
   mkdir build
   cd build
   ```
3. Run CMake to configure the project:
   ```bash
   cmake ..
   ```
4. Build the project:
   ```bash
   cmake --build .
   ```

## Running the Application
After building, you can run the application from the build directory:
```bash
./MyProject
```

## Project Structure
- **src/**: Contains the source code, including entities, repositories, and controllers.
- **qml.qrc**: QML resources for the user interface.
- **CMakeLists.txt**: Build configuration for the project.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
