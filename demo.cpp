{\rtf1\ansi\ansicpg936\cocoartf1671\cocoasubrtf500
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #include \'93demo.h\'94\
int main(int argc, char *argv[])\
\{\
   BtrieveClient btrieveClient(0x4232, 0);\
   Btrieve::StatusCode status = Btrieve::STATUS_CODE_UNKNOWN;\
   BtrieveFile btrieveFile;\
   \
   // If creating the File fails.\
   if ((status = createFile(&btrieveClient)) != Btrieve::STATUS_CODE_NO_ERROR)\
   \{\
      goto leave;\
   \}\
   // If opening the File fails.\
   if ((status = openFile(&btrieveClient, &btrieveFile)) != Btrieve::STATUS_CODE_NO_ERROR)\
   \{\
      goto leave;\
   \}\
   // If loading the File fails.\
   if ((status = loadFile(&btrieveFile)) != Btrieve::STATUS_CODE_NO_ERROR)\
   \{\
      goto leave;\
   \}\
   else \{\
      int c;\
      unsigned int delay = 5000;\
      int loops = -1;\
      bool silent = false;\
      while ((c = getopt(argc, argv, "shd:n:")) != -1) \{\
         switch (c) \{\
         case 'd': \{\
            delay = atoi(optarg);\
            if (delay < 1) \{ delay = 5000; \}\
            break;\
         \}\
         case 'n':\
            loops = atoi(optarg);\
            break;\
         case 's':\
            silent = true;\
            break;\
         default:\
            abort();\
         \}\
      \}\
      if (delay > 0) \{\
         monitoring(delay, loops, silent);\
         \}\
      \}\
   // If closeing the File fails.\
   if ((status = closeFile(&btrieveClient, &btrieveFile)) != Btrieve::STATUS_CODE_NO_ERROR)\
   \{\
      goto leave;\
   \}\
leave:\
   // If there wasn't a failure.\
   if (status == Btrieve::STATUS_CODE_NO_ERROR)\
      return 0;\
   return 1;\
\}}