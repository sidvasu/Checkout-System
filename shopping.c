//Siddharth Vasu
//CPSC 223C, Spring 2023
//Assignment 5
//siddharth.vasu@csu.fullerton.edu

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Function prototypes
void menu();
void show_inventory(char * inventoryfilename);
void view_cart(char * cartfilename);
void purchase_item(char * inventoryfilename, char * cartfilename);
void prepare_receipt(char * cartfilename, char * receiptfilename);

//Main function
int main(void) {
  printf("Welcome to Fullerton Farmer's Market\n");

  //Inputting inventory file name
  char inventory[64];
  printf("Enter name of inventory file: ");
  fgets(inventory, 64, stdin);
  inventory[strlen(inventory)-1] = '\0';
  
  FILE * inventoryfile = fopen(inventory, "r");
  while(inventoryfile == NULL) {
    printf("File was not found, try again: ");
    fgets(inventory, 64, stdin);
    inventory[strlen(inventory)-1] = '\0';
    inventoryfile = fopen(inventory, "r");
  }
  fclose(inventoryfile);

  //Initializing the customer plus shopping cart and receipt files
  int customer_counter = 1;
  char cartfilename[20];
  sprintf(cartfilename, "cart%d.txt", customer_counter);
  FILE * cartfile = fopen(cartfilename, "w");
  fclose(cartfile);

  char receiptfilename[20];
  strcpy(receiptfilename, "ffm.txt");
  FILE * receiptfile = fopen(receiptfilename, "w");
  fclose(receiptfile);

  char selection = 'R';

  //Runs a function when the user makes a valid selection 
  while (toupper(selection) != 'X') {
    switch (toupper(selection)) {
      //Repeats the menu
      case 'R':
        menu();
        break;
      
      //Displays the inventory
      case 'D':
        show_inventory(inventory);
        break;

      //Views the cart
      case 'V':
        view_cart(cartfilename);
        break;

      //Purchases an item from inventory and puts into cart
      case 'P':
        purchase_item(inventory, cartfilename);
        break;

      //Handles invalid selections
      default:
        printf("Invalid selection.");
        break;
    }

    //Prompts the user for a selection
    printf("\n\nPlease make a selection: ");
    selection = fgetc(stdin);
    if (selection == '\n') {
      selection = fgetc(stdin);
    }
  }

  //Prepares the receipt, closes program
  prepare_receipt(cartfilename, receiptfilename);
  printf("\nYour receipt has been generated\n");
  printf("Please view your receipt in the file: ffm.txt");
  printf("\nThank you for shopping with us!\n");
  return 0;
}

//Function implementations
void menu() {
  printf("\nHere is our menu:\n");
  printf("D = Display list of merchandise in inventory\n");
  printf("P = Purchase an item\n");
  printf("R = Repeat this menu\n");
  printf("V = View shopping cart\n");
  printf("X = Exit and generate receipt");
}

void show_inventory(char * inventoryfilename) {
  FILE * inventory = fopen(inventoryfilename, "r");
  char line[1024];
  
  printf("\n");
  while(fgets(line, 1024, inventory) != NULL) {
    fputs(line, stdout);
  }
  fclose(inventory);
}

void view_cart(char * cartfilename) {
  FILE * cart = fopen(cartfilename, "r");
  char line[1024];

  printf("\n");
  while (fgets(line, 1024, cart) != NULL) {
    fputs(line, stdout);
  }
  fclose(cart);
}

void purchase_item (char * inventoryfilename, char * cartfilename) {
  FILE * inventory = fopen(inventoryfilename, "r");
  FILE * cart = fopen(cartfilename, "a");

  char item[128];
  char avail[128];
  int found = 0;
  int quantity = 0;

  printf("\nEnter the name of the item (must match on the first 8 characters): ");
  fgets(item, 128, stdin);
  if (item[0] == '\n') {
    fgets(item, 128, stdin);
  }
  item[strlen(item)-1] = '\0';
  
  //Checks inventory for the item the user inputs
  while ((fgets(avail, 128, inventory)) != NULL && !found) {
    avail[strlen(avail)-1] = '\0';
    
    /*
    Compares user input and current inventory item to see
    if they match on the first 8 characters
    */
    found = (strncmp(item, avail, 8) == 0);

    //Case if found, asks for quantity and adds to cart
    if (found) {
      printf("Please enter quantity: ");
      scanf("%d", &quantity);
      if (quantity > 0) {
        fprintf(cart, "%dx %s\n", quantity, avail);
        printf("Item was added to shopping cart");
      }

      //Doesn't add to cart if input for quantity is invalid
      else {
        printf("\nInvalid amount");
      }
    }
  }

  //Case if the item isn't found
  if (!found) {
    printf("\nSorry that item is out of stock");
  }
  fclose(inventory);
  fclose(cart);
}

void prepare_receipt(char * cartfilename, char * receiptfilename) {
  FILE * cart = fopen(cartfilename, "r");
  FILE * checkout = fopen(receiptfilename, "w");

  //Stores the current date and time
  time_t current_linux_time = time(NULL);
  struct tm *broken = localtime(&current_linux_time);

  //Initial receipt dialog with store name, date, and slogan
  fprintf(checkout, "Fullerton Farmer's Market  ");
  fprintf(checkout, "%02d/%02d/%04d\n", broken->tm_mon + 1, broken->tm_mday, broken->tm_year + 1900);
  fprintf(checkout, "The customer is always first\n\n");
  
  double total = 0.0;
  char temp[128];
  char * token;
  char * ptr;
  long quantity;

  //Puts the contents of cart into checkout, adds prices
  while (fgets(temp, 128, cart) != NULL) {
    fprintf(checkout, "%s", temp);
    quantity = strtol(temp, &ptr, 10);
    token = strstr(temp, "$");
    token++;
    total += quantity * strtod(token, &ptr);
  }

  fprintf(checkout, "\nYour total is: $%1.2lf", total);
  fprintf(checkout, "\nThank you for shopping with us!");

  fclose(cart);
  fclose(checkout);
}

