



int performAction(String inputString) {
  // Tokenize the input string based on space
  char *token = strtok(inputString.begin(), " ");

  // Check if there are tokens
  while (token != NULL) {
    // Check the token and perform actions accordingly
    if (strcmp(token, "forward") == 0) {
      // Action 1
      return 1;
    } else if (strcmp(token, "backward") == 0) {
      // Action 2
      return 2;
    } else if (strcmp(token, "left") == 0) {
      // Action 3
      return 3;
    } else if (strcmp(token, "right") == 0) {
      // Action 4
      return 4;
    }

    // Get the next token
    token = strtok(NULL, " ");
  }

  // Default case: Invalid or unrecognized action
  return 0;
}

// void displayAction(String inputValue){
//   String payload = performAction(inputValue);
//   Serial.println(payload);
// }