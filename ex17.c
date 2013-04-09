#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
    int age;
};

struct Database {
    struct Address rows[MAX_ROWS];
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void Database_close();

void die(const char *message, ...)
{
	va_list ap;
	struct Connection *conn;
	va_start(ap, message);
	conn = va_arg(ap, struct Connection *);
	if(!conn){
		Database_close(conn);
	}
		if(errno){// errno gives the number of the last error, 0 if no error, if(errno) only executes if there has been an error (see sys_errlist[])
			perror(message); // interprets the int returned by errno into something human readable
		} else {//if the error is unknown (not caught by errno), 	return a generic error message
			printf("ERROR: %s\n",message);
		}
	
	va_end(ap);
	
		exit(1);// terminates the function call, releases memory, but memory blocks still reachable (ex16.c)?
	
};

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n",
            addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to load database.");
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error");

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error");

    if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file) die("Failed to open the file");

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
        if(conn->db) free(conn->db);
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to write database.");

    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.");
}

void Database_create(struct Connection *conn)
{
    int i = 0;

    for(i = 0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0, .name = "\0"};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first");

    addr->set = 1;
    // WARNING: bug, read the "How To Break It" and fix this
    char *res = strncpy(addr->name, name, MAX_DATA);
    // demonstrate the strncpy bug
    if(!res) die("Name copy failed");

    res = strncpy(addr->email, email, MAX_DATA);
    if(!res) die("Email copy failed");
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set");
    }
}

void Database_find(struct Connection *conn, char *namef)
{
	struct Address *addr = &conn->db->rows[0];
	int i = 0;
	int test = 0;
	int size = sizeof(namef)/sizeof(char);
	//printf("name: %d, %s\n", sizeof(namef), namef);

	for(i = 0; i < MAX_ROWS; i++){
		if((addr+i)->set){
			//printf("id: %d\n", (addr+i)->id);
			char *name = (char *) malloc(size);//(addr+i)->name;
			strncpy(name, (addr+i)->name, size);
			/*char *name_short = (char *) malloc(size);
			strncpy(name_short, namef, size);*/
			//printf("name: %d, %s\n", sizeof(name_short), name_short);
			int diff = strcmp(name,namef);
			//printf("diff: %d\n", diff);
			if(!diff){
				test = 1;
				//printf("id: %d\n", (addr+i)->id);
				Address_print(addr+i);
			}
		}
	}
	if(test == 0)
	{
		printf("No one by that name.\n");
	}
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;
	char *f = "f";

    if(argc > 3){
    	if(argv[3] != f){
    		id = atoi(argv[3]);
    	} 
    }
    if(id >= MAX_ROWS) die("There's not that many records.");

    switch(action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get");

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set");

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete");

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
            
        case 'f':
        	if(argc != 4) die("Need a name");
        	char *name = (char *) malloc(sizeof(argv[3]));
        	strcpy(name,argv[3]);
        	//printf("%s\n", name);
        	Database_find(conn, name);
        	break;
        
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, f=find");
    }

    Database_close(conn);

    return 0;
}
