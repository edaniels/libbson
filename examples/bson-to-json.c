/*
 * Copyright 2013 10gen Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <bson.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


int
main (int   argc,
      char *argv[])
{
   bson_reader_t reader;
   const bson_t *b;
   const char *filename;
   bson_bool_t eof;
   size_t len;
   char *str;
   int fd;
   int i;

   if (argc == 1) {
      fprintf(stderr, "usage: %s FILE...\n", argv[0]);
      return 1;
   }

   for (i = 1; i < argc; i++) {
      filename = argv[i];

      errno = 0;
      fd = open(filename, O_RDONLY);
      if (fd == -1) {
         fprintf(stderr, "Failed to open %s: %s\n", filename, strerror(errno));
         continue;
      }

      bson_reader_init_from_fd(&reader, fd, TRUE);

      while ((b = bson_reader_read(&reader, &eof))) {
         str = bson_as_json(b, &len);
         fprintf(stdout, "%s\n", str);
         bson_free(str);
      }

      bson_reader_destroy(&reader);
   }

   return 0;
}
