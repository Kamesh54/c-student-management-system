#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Course {
    char name[50];
    int courseId;
    struct Course *left;
    struct Course *right;
} Course;

typedef struct Student {
    char name[50];
    int studentId;
    struct Course *enrolledCourses;
    struct Student *next;
} Student;

void saveCoursesToFileHelper(Course *node, FILE *file);
Course *insertIntoBST(Course *root, Course *newCourse);
Course *deleteNode(Course *root);

Student *createStudent(int studentId, const char *name) {
    Student *newStudent = (Student *)malloc(sizeof(Student));
    newStudent->studentId = studentId;
    strncpy(newStudent->name, name, sizeof(newStudent->name) - 1);
    newStudent->enrolledCourses = NULL;
    newStudent->next = NULL;
    return newStudent;
}

Course *createCourse(int courseId, const char *name) {
    Course *newCourse = (Course *)malloc(sizeof(Course));
    newCourse->courseId = courseId;
    strncpy(newCourse->name, name, sizeof(newCourse->name) - 1);
    newCourse->left = newCourse->right = NULL;
    return newCourse;
}

void enrollStudent(Student *student, Course *course) {
    Course *current = student->enrolledCourses;
    while (current != NULL) {
        if (current->courseId == course->courseId) {
            printf("Student is already enrolled in this course.\n");
            return;
        }
        current = current->left;
    }
    Course *newEnrollment = createCourse(course->courseId, course->name);
    newEnrollment->left = student->enrolledCourses;
    student->enrolledCourses = newEnrollment;
    printf("Student successfully enrolled in %s.\n", course->name);
}

void displayEnrolledCourses(Student *student) {
    Course *current = student->enrolledCourses;
    printf("Enrolled Courses for Student %s (ID-> %d):\n", student->name, student->studentId);
    printf("*****Courses******\n");
    while (current != NULL) {
        printf(" %s\n", current->name);
        current = current->left;
    }
    printf("\n");
}

void displayAllCourses(Course *root) {
    if (root != NULL) {
        displayAllCourses(root->left);
    printf("| %d          |  %s  \n", root->courseId, root->name);
        displayAllCourses(root->right);
    }
}

void displayAllStudents(Student *students) {
    while (students != NULL) {
        printf("|%d        |  %s  \n", students->studentId, students->name);
        students = students->next;
    }
}

int checkPrerequisites(Student *student, Course *course) {
    char questions[][100] = {
        "What is the capital of France?",
        "Which planet is known as the Red Planet?",
        "What is the largest mammal on Earth?",
    };

    char answers[][100] = {
        "Paris",
        "Mars",
        "Whale",
    };

    int numQuestions = sizeof(questions) / sizeof(questions[0]);
    for (int i = 0; i < numQuestions; ++i) {
        printf("%s\n", questions[i]);

        char userAnswer[100];
        printf("Your Answer: ");
        scanf("%s", userAnswer);
        if (strcasecmp(userAnswer, answers[i]) != 0) {
            printf("\n******INCORRECT****** Prerequisites not satisfied\n");
            return 0;
        }
    }
    printf("\n******CORRECT****** Prerequisites are satisfied\n");
    return 1; 
}

int getValidIntInput(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            break;
        } else {
            while (getchar() != '\n');
            printf("\nw******INVALID INPUT****** Please enter a valid integer\n");
        }
    }
    return value;
}

Student *searchStudentByName(Student *students, const char *name) {
    while (students != NULL) {
        if (strcmp(students->name, name) == 0) {
            return students;
        }
        students = students->next;
    }
    return NULL;
}

Course *searchCourseByName(Course *courses, const char *name) {
    if (courses == NULL) {
        return NULL;
    }
    int compareResult = strcmp(name, courses->name);
    if (compareResult == 0) {
        return courses;
    } else if (compareResult < 0) {
        return searchCourseByName(courses->left, name);
    } else {
        return searchCourseByName(courses->right, name);
    }
}

Student *findMinStudent(Student *start) {
    Student *minStudent = start;
    Student *current = start->next;

    while (current != NULL) {
        if (current->studentId < minStudent->studentId) {
            minStudent = current;
        }
        current = current->next;
    }

    return minStudent;
}

void swapStudents(Student *a, Student *b) {
    int tempId = a->studentId;
    a->studentId = b->studentId;
    b->studentId = tempId;

    char tempName[50];
    strncpy(tempName, a->name, sizeof(tempName) - 1);
    strncpy(a->name, b->name, sizeof(a->name) - 1);
    strncpy(b->name, tempName, sizeof(b->name) - 1);
}
void selectionSortStudentsById(Student *start) {
    Student *current = start;
    while (current != NULL) {
        Student *minStudent = findMinStudent(current);
        swapStudents(current, minStudent);
        current = current->next;
    }
}

void sortStudentsById(Student *students) {
    selectionSortStudentsById(students);
}

void sortCoursesByName(Course *courses) {
    int swapped;
    Course *ptr1;
    Course *lptr = NULL;
    if (courses == NULL) {
        return;
    }
    do {
        swapped = 0;
        ptr1 = courses;

        while (ptr1->right != lptr) {
            if (strcmp(ptr1->name, ptr1->right->name) > 0) {
                int tempId = ptr1->courseId;
                ptr1->courseId = ptr1->right->courseId;
                ptr1->right->courseId = tempId;

                char tempName[50];
                strncpy(tempName, ptr1->name, sizeof(ptr1->name) - 1);
                strncpy(ptr1->name, ptr1->right->name, sizeof(ptr1->name) - 1);
                strncpy(ptr1->right->name, tempName, sizeof(ptr1->right->name) - 1);

                swapped = 1;
            }
            ptr1 = ptr1->right;
        }
        lptr = ptr1;
    } while (swapped);
}

void deleteStudent(Student **students, int studentId) {
    Student *current = *students;
    Student *prev = NULL;

    while (current != NULL) {
        if (current->studentId == studentId) {
            if (prev == NULL) {
                *students = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Student with ID %d deleted.\n", studentId);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Student with ID %d not found.\n", studentId);
}

void deleteCourse(Course **root, int courseId) {
    if (*root == NULL) {
        printf("Course with ID %d not found.\n", courseId);
        return;
    }

    if ((*root)->courseId == courseId) {
        Course *temp = *root;
        *root = deleteNode(*root);
        free(temp);
        printf("Course with ID %d deleted.\n", courseId);
    } else if ((*root)->courseId > courseId) {
        deleteCourse(&((*root)->left), courseId);
    } else {
        deleteCourse(&((*root)->right), courseId);
    }
}

Course *findMin(Course *node) {
    Course *current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

Course *deleteNode(Course *root) {
    if (root->left == NULL) {
        Course *temp = root->right;
        return temp;
    } else if (root->right == NULL) {
        Course *temp = root->left;
        return temp;
    }
    Course *temp = findMin(root->right);
    root->courseId = temp->courseId;
    strncpy(root->name, temp->name, sizeof(root->name) - 1);

    root->right = deleteNode(root->right);

    return root;
}

void saveStudentsToFile(Student *students, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for writing.\n", filename);
        return;
    }

    while (students != NULL) {
        fprintf(file, "%d %s\n", students->studentId, students->name);
        students = students->next;
    }

    fclose(file);
}

Student *loadStudentsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for reading.\n", filename);
        return NULL;
    }

    Student *head = NULL;
    int studentId;
    char name[50];

    while (fscanf(file, "%d %s", &studentId, name) == 2) {
        Student *newStudent = createStudent(studentId, name);
        newStudent->next = head;
        head = newStudent;
    }

    fclose(file);
    return head;
}

void saveCoursesToFile(Course *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for writing.\n", filename);
        return;
    }

    saveCoursesToFileHelper(root, file);

    fclose(file);
}

void saveCoursesToFileHelper(Course *node, FILE *file) {
    if (node != NULL) {
        saveCoursesToFileHelper(node->left, file);
        fprintf(file, "%d %s\n", node->courseId, node->name);
        saveCoursesToFileHelper(node->right, file);
    }
}

Course *loadCoursesFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s for reading.\n", filename);
        return NULL;
    }

    Course *root = NULL;
    int courseId;
    char name[50];

    while (fscanf(file, "%d %s", &courseId, name) == 2) {
        Course *newCourse = createCourse(courseId, name);
        root = insertIntoBST(root, newCourse);
    }

    fclose(file);
    return root;
}

Course *insertIntoBST(Course *root, Course *newCourse) {
    if (root == NULL) {
        return newCourse;
    }

    if (newCourse->courseId < root->courseId) {
        root->left = insertIntoBST(root->left, newCourse);
    } else if (newCourse->courseId > root->courseId) {
        root->right = insertIntoBST(root->right, newCourse);
    }

    return root;
}

int main() {
    Student *students = loadStudentsFromFile("students.txt");
    Course *courses = loadCoursesFromFile("courses.txt");

    while (1) {
        printf("\nOptions:\n");
        printf("1.  Create Student\n");
        printf("2.  Create Course\n");
        printf("3.  Enroll Student in Course\n");
        printf("4.  Display Enrolled Courses for Student\n");
        printf("5.  Display All Available Courses\n");
        printf("6.  display All Students\n");
        printf("7.  Search Student by Name\n");
        printf("8.  Search Course by Name\n");
        printf("9.  Sort Students by Number\n");
        printf("10. Sort Courses by Name\n");
        printf("11. Delete student by ID\n");
        printf("12. Delete course by ID\n");
        printf("13. Exit\n");

        int choice = getValidIntInput("Enter your choice-> ");

        switch (choice) {
            case 1: {
                int studentId = getValidIntInput("Enter Student ID-> ");
                char studentName[50];
                printf("Enter Student Name-> ");
                scanf("%s", studentName);
                Student *newStudent = createStudent(studentId, studentName);
                newStudent->next = students;
                students = newStudent;

                printf("Student %s (ID-> %d) created.\n", newStudent->name, newStudent->studentId);
                saveStudentsToFile(students, "students.txt");
                break;
            }

            case 2: {
                int courseId = getValidIntInput("Enter Course ID-> ");
                char courseName[50];
                printf("Enter Course Name-> ");
                scanf("%s", courseName);
                Course *newCourse = createCourse(courseId, courseName);
                courses = insertIntoBST(courses, newCourse);
                printf("Course %s (ID-> %d) created.\n", newCourse->name, newCourse->courseId);
                saveCoursesToFile(courses, "courses.txt");
                break;
            }

            case 3: {
    int studentId = getValidIntInput("Enter Student ID-> ");
    int courseId = getValidIntInput("Enter Course ID-> ");
    Student *currentStudent = students;
    while (currentStudent != NULL) {
        if (currentStudent->studentId == studentId) {
            break;
        }
        currentStudent = currentStudent->next;
    }
    Course *currentCourse = courses;
    while (currentCourse != NULL) {
        if (currentCourse->courseId == courseId) {
            break;
        }
        int compareResult = courseId - currentCourse->courseId;
        if (compareResult < 0) {
            currentCourse = currentCourse->left;
        } else if (compareResult > 0) {
            currentCourse = currentCourse->right;
        } else {
            printf("Course with the same ID already exists.\n");
            break;
        }
    }

    if (currentStudent == NULL || currentCourse == NULL) {
        printf("Student or course not found.\n");
    } else {
        if (checkPrerequisites(currentStudent, currentCourse)) {
            fflush(stdout);
            for (int i = 0; i < 15; ++i) {
                printf(".");
                fflush(stdout);
                sleep(1);;
                }
                printf("\n");
            enrollStudent(currentStudent, currentCourse);
        } else {
            printf("Prerequisites not satisfied.\n");
        }
    }
    break;
}
            case 4: {
                int studentId = getValidIntInput("Enter Student ID-> ");
                Student *currentStudent = students;
                while (currentStudent != NULL) {
                    if (currentStudent->studentId == studentId) {
                        break;
                    }
                    currentStudent = currentStudent->next;
                }

                if (currentStudent == NULL) {
                    printf("Student not found.\n");
                } else {
                    displayEnrolledCourses(currentStudent);
                }
                break;
            }

            case 5:
                printf("All Available Courses->\n");
                printf("*---Course ID---|---- Name---*\n");
                displayAllCourses(courses);
                break;

            case 6:
                    printf("All Students->\n");
                    printf("#---Student ID-----#----Name----#\n");
                displayAllStudents(students);
                break;

            case 7: {
                char searchName[50];
                printf("Enter student name to search-> ");
                scanf("%s", searchName);

                Student *foundStudent = searchStudentByName(students, searchName);

                if (foundStudent != NULL) {
                    printf("Student found: %s (ID-> %d)\n", foundStudent->name, foundStudent->studentId);
                } else {
                    printf("!!!!!--Student not found.--!!!!!\n");
                }
                break;
            }

            case 8: {
                char searchName[50];
                printf("Enter course name to search-> ");
                scanf("%s", searchName);

                Course *foundCourse = searchCourseByName(courses, searchName);

                if (foundCourse != NULL) {
                    printf("Course found: %s (ID-> %d)\n", foundCourse->name, foundCourse->courseId);
                } else {
                    printf("Course not found.\n");
                }
                break;
            }

            case 9:
                sortStudentsById(students);
                printf("Students sorted by ID.\n");
                break;

            case 10:
                sortCoursesByName(courses);
                printf("Courses sorted by name.\n");
                break;
            case 11: {
                int studentId = getValidIntInput("Enter Student ID to delete-> ");
                deleteStudent(&students, studentId);
                break;
                }

            case 12: {
                int courseId = getValidIntInput("Enter Course ID to delete-> ");
                deleteCourse(&courses, courseId);
                break;
                }

            case 13:
                saveStudentsToFile(students, "students.txt");
                saveCoursesToFile(courses, "courses.txt");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}


