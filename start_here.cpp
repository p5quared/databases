//
// Created by Peter Vaiciulis on 4/10/23.
//

#include <sqlite3.h>
#include <iostream>

// clang++ -std=c++17 -stdlib=libc++ start_here.cpp -o intro
/*
 * Run the above command to compile this file; do you notice anything wrong?
 *
 * You likely got a linker error. This is because we are using a library that is not part of the standard library.
 * You probably already have it installed though so we can probably link it by simply adding the -lsqlite3 flag.
 *
 * Try running it again with the flag and it should work.
 *
 * After you have it working, you should see that our database connection was successful,
 * and we have a database file called test.db in our current directory!
 * */
int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("test.db", &db);

   if( rc ) {
      std::cout << "Database connection error: " << sqlite3_errmsg(db) << std::endl;
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   sqlite3_close(db);
    return 0;
}