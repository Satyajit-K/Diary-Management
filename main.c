#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct login_sys{
  char username[100];
  char password[100];
  char fname[20];
  char lname[20];
};

struct login_sys user;
char login_file[] = "login_details.txt";
char temp_file[] = "temp_file.txt";
char counts_file[] = "counts.txt";

void update_counts(char *username, int choice) {
  FILE* old_counts = fopen(counts_file, "r");
  
  // Create empty file if not exists
  if (!old_counts) {
    old_counts = fopen(counts_file, "w");
    fclose(old_counts);
    old_counts = fopen(counts_file, "r");
  }

  FILE* new_counts = fopen(temp_file, "w");

  char name[100];
  int total, personal, professional;
  
  int t, pe, pr;
  int found = 0;
  while (1) {
    int end = fscanf(old_counts, "%s %d %d %d", name, &total, &personal, &professional);
    if (end == EOF) break;
    if (strcmp(username, name) == 0) {
        found = 1;
        t = total;
        pe = personal;
        pr = professional;
    } else {
        fprintf(new_counts, "%s %d %d %d\n", name, total, personal, professional);
    }
  }

  if (!found) {
      pe = pr = t = 0;
  }
  if (choice == 1) {
      pe++;
  } else {
      pr++;
  }
  t = pe + pr;

  fprintf(new_counts, "%s %d %d %d\n", username, t, pe, pr);

  fclose(old_counts);
  fclose(new_counts);

  remove(counts_file);
  rename(temp_file, counts_file);
}

void get_counts(char *username, int *total, int *personal, int *professional) {
    FILE* fp = fopen(counts_file, "r");
    char name[100];
    int t, pe, pr;
    int found = 0;
    while (1) {
        int end = fscanf(fp, "%s %d %d %d", name, &t, &pe, &pr);
        if (end == EOF) break;
        if (strcmp(username, name) == 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        t = pe = pr = 0;
    }
    fclose(fp);
    *total = t;
    *personal = pe;
    *professional = pr;
}

void new_entry(char *username) {
    printf("1 -> Personal entry\n2 -> Professional entry\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
      case 1:
      case 2:
        update_counts(username, choice);
        break;
      default:
        printf("ERROR: Incorrect choice! Please try again!");
        return;
    }

    char filename[1000];
    strcpy(filename, username);
    strcat(filename, "-diary.txt");

    FILE *fp = fopen(filename, "a");
    if (choice == 1) {
      fprintf(fp, "PERSONAL\n");
    } else {
      fprintf(fp, "PROFESSIONAL\n");
    }
    
    printf("Enter diary entry (type --- on a line to end):\n\n");
    char line[1000] = "";
    do {
        scanf(" %[^\n]", line);
        fprintf(fp, "%s\n", line);
    } while (strcmp(line, "---") != 0);
    fprintf(fp, "\n");
    fclose(fp);
}

void view_entry(char *username) {
    char filename[1000];
    strcpy(filename, username);
    strcat(filename, "-diary.txt");

    FILE *fp = fopen(filename, "r");

    int total, personal, professional;
    get_counts(username, &total, &personal, &professional);

    double per = (personal * 100.0) / total;
    double pro = (professional * 100.0) / total;

    printf("\n\n****************************\n\n");

    printf("Personal %%: %.2f%%\nProfessional %%: %.2f%%", per, pro);

    printf("\n\n****************************\n\n");

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
    }

    printf("\n\n============================\n\n");;

    fclose(fp);
}

void launch_app(char *username, char *fname) {
  int ch;
  do {
    printf("\nWelcome %s\n\n", fname);
    printf("[1] New entry\n[2] View entry\n[3] Signout\n\n");
    printf("Enter your choice: ");
    scanf("%d", &ch);

    switch (ch) {
      case 1:
        new_entry(username);
        break;
      case 2:
        view_entry(username);
        break;
      case 3:
        printf("Successfully logged out!\n\n");
        return;
        break;
      default:
        printf("\n\nERROR: Invalid choice! Please try again!\n");
    }
  } while (1);
}

void signup() {
  FILE* old_login = fopen(login_file, "r");
  
  // Create empty file if not exists
  if (!old_login) {
    old_login = fopen(login_file, "w");
    fprintf(old_login, "0\n");
    fclose(old_login);
    old_login = fopen(login_file, "r");
  }

  FILE* new_login = fopen(temp_file, "w");

  int user_count;
  fscanf(old_login, "%d", &user_count);

  fprintf(new_login, "%d\n", user_count + 1);
  for (int i = 0; i < user_count; ++i) {
    fscanf(old_login, "%s", user.username);
    fscanf(old_login, "%s", user.password);
    fscanf(old_login, "%s", user.fname);
    fscanf(old_login, "%s", user.lname);

    fprintf(new_login, "%s\n", user.username);
    fprintf(new_login, "%s\n", user.password);
    fprintf(new_login, "%s\n", user.fname);
    fprintf(new_login, "%s\n", user.lname);
  }

  // Take input of new user registration

  printf("\n\nNEW REGISTRATION:\n\n");
  printf("Enter username: ");
  scanf("%s", user.username);
  printf("Enter password: ");
  scanf("%s", user.password);
  printf("Enter your first name: ");
  scanf("%s", user.fname);
  printf("Enter your last name: ");
  scanf("%s", user.lname);

  fprintf(new_login, "%s\n", user.username);
  fprintf(new_login, "%s\n", user.password);
  fprintf(new_login, "%s\n", user.fname);
  fprintf(new_login, "%s\n", user.lname);

  fclose(old_login); fclose(new_login);

  remove(login_file);
  rename(temp_file, login_file);
}

void signin() {
  printf("\n\nLOGIN\n\n");

  char username[100];
  char password[100];
  char correct_password[100];
  printf("Enter username: ");
  scanf("%s", username);

  FILE* fp = fopen(login_file, "r");
  int user_count;
  fscanf(fp, "%d", &user_count);

  int found = 0;
  for (int i = 0; i < user_count; ++i) {
    fscanf(fp, "%s", user.username);
    fscanf(fp, "%s", user.password);
    fscanf(fp, "%s", user.fname);
    fscanf(fp, "%s", user.lname);

    if (strcmp(user.username, username) == 0) {
      strcpy(correct_password, user.password);
      found = 1;
      break;
    }
  }
  fclose(fp);

  if (!found) {
    printf("\n\nERROR: User does not exist!\n\n");
    return;
  }

  printf("Enter password: ");
  scanf("%s", password);

  if (strcmp(password, correct_password) != 0) {
    printf("\n\nERROR: Invalid password!\n\n");
    return;
  }

  //Launch rest of the app

  launch_app(username, user.fname);
}

int main(void) {
  int ch;
  do {
    printf("\nWelcome to UrDiary\n");
    printf("\n[1] Signup\n[2] Signin\n[3] Exit");
    printf("\n\nEnter your choice: ");

    scanf("%d",&ch);

    switch (ch) {
      case 1:
        signup();
        break;
      case 2:
        signin();
        break;
      case 3:
        exit(0);
        break;
      default:
        printf("Enter a valid choice!");
        break;
    }
  } while (1);
  return 0;
}