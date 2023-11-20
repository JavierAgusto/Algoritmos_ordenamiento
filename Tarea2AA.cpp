// CARPETA 'DATOS' QUE CONTIENE LOS .TXT DEBE ESTAR EN LA RAÍZ DEL PROYECTO

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;
using namespace std::chrono;

// Funciones de ordenamiento

void countingSort(std::vector<int> &arr) { // O(n+k)
  int max_num = *std::max_element(arr.begin(), arr.end());
  std::vector<int> count(max_num + 1, 0);

  for (int num : arr) {
    count[num]++;
  }

  int index = 0;
  for (int i = 0; i <= max_num; ++i) {
    while (count[i] > 0) {
      arr[index++] = i;
      count[i]--;
    }
  }
}

void heapify(std::vector<int> &arr, int n, int i) { // O(log n)
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < n && arr[left] > arr[largest]) {
    largest = left;
  }

  if (right < n && arr[right] > arr[largest]) {
    largest = right;
  }

  if (largest != i) {
    std::swap(arr[i], arr[largest]);
    heapify(arr, n, largest);
  }
}

void heapSort(std::vector<int> &arr) { // O(n log n)
  int n = arr.size();

  for (int i = n / 2 - 1; i >= 0; --i) {
    heapify(arr, n, i);
  }

  for (int i = n - 1; i > 0; --i) {
    std::swap(arr[i], arr[0]);
    heapify(arr, i, 0);
  }
}

void bucketSort(std::vector<int> &arr) { // O(n^2)
  int n = arr.size();
  int max_num = *std::max_element(arr.begin(), arr.end());
  int min_num = *std::min_element(arr.begin(), arr.end());
  int bucket_size = (max_num - min_num) / n + 1;

  std::vector<std::vector<int>> buckets(n);
  for (int i = 0; i < n; ++i) {
    int bucket_index = (arr[i] - min_num) / bucket_size;
    buckets[bucket_index].push_back(arr[i]);
  }

  for (int i = 0; i < n; ++i) {
    std::sort(buckets[i].begin(), buckets[i].end());
  }

  int index = 0;
  for (int i = 0; i < n; ++i) {
    for (int num : buckets[i]) {
      arr[index++] = num;
    }
  }
}

void flip(std::vector<int> &arr, int i) { // O(n)
  std::reverse(arr.begin(), arr.begin() + i + 1);
}

void pancakeSort(std::vector<int> &arr) { // O(n^2)
  int n = arr.size();

  for (int curr_size = n; curr_size > 1; --curr_size) {
    int max_index =
        std::max_element(arr.begin(), arr.begin() + curr_size) - arr.begin();

    if (max_index != curr_size - 1) {
      flip(arr, max_index);
      flip(arr, curr_size - 1);
    }
  }
}

// Funciones de manejo de archivos

std::vector<int> readDataFromFile(const std::string &file_path) { // O(n)
  try {
    std::ifstream file(file_path);
    if (!file.is_open()) {
      throw std::runtime_error("El archivo '" + file_path +
                               "' no fue encontrado.");
    }

    std::vector<int> data;
    int num;
    while (file >> num) {
      data.push_back(num);
    }

    return data;
  } catch (const std::ifstream::failure &e) {
    std::cerr << "Error al leer el archivo '" << file_path << "': " << e.what()
              << '\n';
    return {};
  }
}

void writeSolutionToFile(const std::vector<int> &sorted_data,
                         int algorithm_code) { // O(n)
  try {
    std::ofstream file("solution_algorithm_" + std::to_string(algorithm_code) +
                       ".txt");
    if (!file.is_open()) {
      throw std::runtime_error("Error al abrir el archivo de solución.");
    }

    for (int num : sorted_data) {
      file << num << ' ';
    }

    // No es necesario cerrar el archivo explícitamente, se cerrará
    // automáticamente al salir del ámbito.
  } catch (const std::ofstream::failure &e) {
    std::cerr << "Error al escribir la solución en el archivo: " << e.what()
              << '\n';
  }
}

// Funciones de interfaz de usuario

std::string getSelectedFile(const std::vector<std::string> &files) { // O(n)
  std::cout << "Archivos disponibles:\n";
  for (size_t i = 0; i < files.size(); ++i) {
    std::cout << i + 1 << ". " << files[i] << '\n';
  }

  try {
    size_t choice;
    std::cout << "\nSeleccione el número de archivo para ordenar: ";
    std::cin >> choice;

    if (choice < 1 || choice > files.size()) {
      throw std::out_of_range("Selección fuera de rango.");
    }

    return files[choice - 1];
  } catch (const std::exception &e) {
    std::cerr << "Selección inválida: " << e.what() << '\n';
    return "";
  }
}

int getAlgorithmCode() { // O(1)
  try {
    int algorithm_code;
    std::cout << "Seleccione el algoritmo de ordenamiento \n1: Counting \n2: "
                 "Heap \n3: Bucket \n4: Pancake \n5: Salir del programa "
                 "\nOpcion seleccionada: ";
    std::cin >> algorithm_code;

    if (algorithm_code < 1 || algorithm_code > 5) {
      throw std::out_of_range("Código de algoritmo inválido.");
    }
    return algorithm_code;
  } catch (const std::exception &e) {
    std::cerr << "Entrada inválida: " << e.what() << '\n';
    return -1;
  }
}

bool wantToSeeFileContent() { // O(1)
  std::cout << "¿Desea ver el contenido del archivo? \n1: Sí \n2: No \nOpcion "
               "seleccionada : ";
  int choice;
  std::cin >> choice;
  return choice == 1;
}

bool wantToContinue() { // O(1)
  std::cout << "¿Desea volver al menú principal? \n1: Sí \n2: No \nOpcion "
               "seleccionada : ";
  int choice;
  std::cin >> choice;
  return choice == 1;
}

void displayFileContent(const std::vector<int> &data,
                        const std::string &fileName) { // O(n)
  std::cout << "\nDatos originales del archivo " << fileName << ": ";
  for (int num : data) {
    std::cout << num << ' ';
  }
}

int main() {
  std::string folder_path = "datos";
  std::vector<std::string> files;

  for (const auto &entry : fs::directory_iterator(folder_path)) {
    if (entry.path().extension() == ".txt") {
      files.emplace_back(entry.path().filename().string());
    }
  }

  if (files.empty()) {
    std::cout << "No hay archivos .txt en la carpeta.\n";
    return 0;
  }

  while (true) {
    int algorithm_code;
    do {
      algorithm_code = getAlgorithmCode();
      if (algorithm_code == -1) {
        std::cout << "Código de algoritmo inválido.\n";
      } else if (algorithm_code == 5) {
        std::cout << "Saliendo del programa. ¡Hasta luego!\n";
        return 0;
      }
    } while (algorithm_code == -1);

    std::string selected_file = getSelectedFile(files);
    if (selected_file.empty()) {
      break;
    }

    if (wantToSeeFileContent()) {
      std::string file_path = folder_path + "/" + selected_file;
      std::vector<int> data = readDataFromFile(file_path);

      if (data.empty()) {
        return 0;
      }

      displayFileContent(data, selected_file);
    }

    auto start_time = high_resolution_clock::now();

    std::vector<int> data = readDataFromFile(folder_path + "/" + selected_file);
    switch (algorithm_code) {
    case 1:
      countingSort(data);
      break;
    case 2:
      heapSort(data);
      break;
    case 3:
      bucketSort(data);
      break;
    case 4:
      pancakeSort(data);
      break;
    case 5:
      return 0;
    default:
      break;
    }

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    std::cout << "\nTiempo transcurrido en milisegundos: " << duration.count()
              << " milisegundos.\n Tiempo en segundos: "
              << duration.count() / 1000.0 << " segundos.\n Tiempo en minutos: "
              << duration.count() / 60000.0 << " minutos.\n";

    writeSolutionToFile(data, algorithm_code);
    std::cout << "\nSolución guardada en solution_algorithm_" << algorithm_code
              << ".txt\n";

    if (!wantToContinue()) {
      std::cout << "Saliendo del programa. ¡Hasta luego!\n";
      break;
    }
  }

  return 0;
}
