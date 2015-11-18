#include "queue.c"
#include "hashmap.c"

void readDB()
{
    FILE *fp = fopen("TwitterDB.txt", "r");
    const char s[2] = ",";
    char *token;
    int i;
    if(fp != NULL)
    {
        char line[50];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            token = strtok(line, s);
            char name[20] = "";
            strcpy(name, token);
            char def[100] = "";
            while(token != NULL) {
                token = strtok(NULL,s);
                if(token != NULL) {
                    strcat(def, token);
                    strcat(def, ",");
                }
            }
            def[strlen(def)-1] = 0;
            install(name, def);
        }
        fclose(fp);
    } else {
        perror("TwitterDB.txt");
    }   
}

void push_queue() {
    FILE *fp = fopen("client3.in", "r");
    char *token;
    if(fp != NULL)
    {
        char line[50];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            if (line[0] != '\n') {
            
                char *client = (char*)malloc(sizeof(char)*10);
                strcpy(client, line);
                Enqueue(client);
                Print();
            }
            
        }
        fclose(fp);
    } else {
        perror("client3.in");
    }
}

int main() {
    
    readDB();
    push_queue();
    
    char name1[] = "Minneapolis";
    char name2[] = "Paris";
    char name3[] = "London";
    struct nlist* result = lookup(name1);
    struct nlist* result2 = lookup(name2);
    struct nlist* result3 = lookup(name3);
   // printf("%s", result->defn);
   // printf("%s", result2->defn);
   // printf("%s", result3->defn);
}
