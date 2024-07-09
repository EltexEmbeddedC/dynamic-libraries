#include <dlfcn.h>
#include <stdio.h>
#include "lib/calc.h"

void print_menu();
void perform_operation(int choice,
                       int (*add)(int, int),
                       int (*sub)(int, int),
                       int (*mul)(int, int),
                       double (*div)(int, int, short*));

int main() {
  int choice;

  void* handle = dlopen("./libcalc.so", RTLD_LAZY);
  if (!handle) {
    fprintf(stderr, "Ошибка: не удалось загрузить библиотеку: %s\n", dlerror());
    return 1;
  }

  int (*add)(int, int) = dlsym(handle, "add");
  int (*sub)(int, int) = dlsym(handle, "sub");
  int (*mul)(int, int) = dlsym(handle, "mul");
  double (*div)(int, int, short*) = dlsym(handle, "div");

  char* error;
  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "Ошибка: не удалось получить адрес функции: %s\n", error);
    dlclose(handle);
    return 1;
  }

  do {
    print_menu();
    scanf("%d", &choice);
    if (choice != 5) {
      perform_operation(choice, add, sub, mul, div);
    }
  } while (choice != 5);

  dlclose(handle);

  return 0;
}

void print_menu() {
  printf(
      "1) Сложение\n"
      "2) Вычитание\n"
      "3) Умножение\n"
      "4) Деление\n"
      "5) Выход\n");
}

void perform_operation(int choice,
                       int (*add)(int, int),
                       int (*sub)(int, int),
                       int (*mul)(int, int),
                       double (*div)(int, int, short*)) {
  int a, b;
  short error;
  double result;

  switch (choice) {
    case 1:
      printf("Введите два целых числа для сложения: ");
      scanf("%d %d", &a, &b);
      printf("Результат: %d + %d = %d\n", a, b, add(a, b));
      break;
    case 2:
      printf("Введите два целых числа для вычитания: ");
      scanf("%d %d", &a, &b);
      printf("Результат: %d - %d = %d\n", a, b, sub(a, b));
      break;
    case 3:
      printf("Введите два целых числа для умножения: ");
      scanf("%d %d", &a, &b);
      printf("Результат: %d * %d = %d\n", a, b, mul(a, b));
      break;
    case 4:
      printf("Введите два целых числа для деления: ");
      scanf("%d %d", &a, &b);
      result = div(a, b, &error);
      if (error) {
        printf("Ошибка: деление на ноль\n");
      } else {
        printf("Результат: %d / %d = %f\n", a, b, result);
      }
      break;
    case 5:
      printf("Выходим...\n");
      break;
    default:
      printf("Ошибка! Неверное значение пункта меню.\n");
      break;
  }
}
