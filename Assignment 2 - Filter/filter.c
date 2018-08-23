#include <unistd.h> /* exit */
#include <getopt.h> /* getopt */
#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, free*/

/*! used for comparison and make code easier to read */
#define TAB '\t'

/*! used for comparison and make code easier to read */
#define NEWLINE '\n'

/*! default number of spaces to use if expand tabs flag is set, but no value
 *  was given */
#define SPACES 8

/* FLAGS! */

/* remove blank lines */
static int fRemoveBlank;

/* delete */
static int fDelete;
/* what char to delete */
static char deleteChar;

/* help screen */
static int fHelp;

/* lower case option */
static int fToLower;

/* displaying line numbers */
static int fLineNo;

/* replace, and the characters to use in replace */
static int fReplace;

/* replace[0] is to be replaced, replace[1] is replacing it */
static char *replaceChars;

/* upper case */
static int fToUpper;

/* expanding tabs and number of spaces to replace tabs w/ */
static int fTabExpand;
static int tabExpandNo;

/* number of non_options (potential files) */
static int nonOptions;

/* array of files to filter, dynamically allocated if files passed */
static const char **Files;

/* STATIC FUNCTIONS WITH COMMENTS */

/*****************************************************************************/
/*!
    \brief
      Displays the help screen.

*/
/*****************************************************************************/
static void HelpScreen(void)
{
  /* prints out help screen */
  printf("Usage: filter [options] [file1 file2 ...] \n\n");
  printf("Options:\n");
  printf(" -b  --remove-blank-lines  removes empty lines.\n");
  printf(" -d  --delete=X            deletes all occurrences of char X.\n");
  printf(" -h  --help                display this information.\n");
  printf(" -l  --tolower             convert all characters to lower case.\n");
  printf(" -n  --line-numbers        prepend line numbers to each line.\n");
  printf(" -r  --replace=XY          replace all char X with char Y.\n");
  printf(" -u  --toupper             convert all characters to upper case.\n");
  printf(" -x  --expand-tabs[=X]     convert tabs to X spaces (default is 8).");
  printf("\n\n");
}

/*****************************************************************************/
/*!
    \brief
      Parse the command line, turning on the options provided and
      capturing the number of non-options provided and dynamically allocates
      array to store them in.
    
    \param argc
      Number of arguments passed.

    \param argv
      Array of strings, nul-terminated, the options of argc.
      
*/
/*****************************************************************************/
static void Parse(int argc, char **argv)
{
  /* used for the result of getopt */
  int resultOpt;

  while (1)
  {
    int Option = 0;

    /* strings to use in comparison of result of getopt */
    const char* Delete = "delete";
    const char* Replace = "replace";
    const char* ExpandTabs = "expand-tabs";

    /* struct to define long options and flags to set */
    static struct option LongOptions[] =
    {
      { "remove-blank-lines", no_argument,       &fRemoveBlank,  1 },
      { "delete",             required_argument, &fDelete,       1 },
      { "help",               no_argument,       &fHelp,         1 },
      { "tolower",            no_argument,       &fToLower,      1 },
      { "line-numbers",       no_argument,       &fLineNo,       1 },
      { "replace",            required_argument, &fReplace,      1 },
      { "toupper",            no_argument,       &fToUpper,      1 },
      { "expand-tabs",        optional_argument, &fTabExpand,    1 },
      { NULL,                 0,                 NULL,           0 }
    };

    /* get argument from command line */
    resultOpt = getopt_long(argc, argv, "-:bd:hlnr::ux:", LongOptions, &Option);

    /* stop at end of command line */
    if (resultOpt == -1)
    {
      break;
    }

    switch (resultOpt)
    {
      /* long option returned */
    case 0:

      /* if delete capture argument to delete to global variable */
      if (LongOptions[Option].name == Delete)
      {
        deleteChar = *optarg;
      }
      /* if replace, capture arguments in global variable */
      if (LongOptions[Option].name == Replace)
      {
        replaceChars = optarg;
      }
      /* if expanding tabs capture argument to get number of spaces
       * to expand tabs with */
      if (LongOptions[Option].name == ExpandTabs)
      {
        /* if argument provided, capture it, else use default number of  */
        if (optarg)
        {
          TabExpandNo = atoi(optarg);
        }
        else
        {
          TabExpandNo = SPACES;
        }
      }

      break;

      /* remove blank lines short option, set the flag */
    case 'b':
      fRemoveBlank = 1;
      break;

      /* delete character short option, set the flag
      * and get character to delete */
    case 'd':
      fDelete = 1;
      deleteChar = *optarg;
      break;

      /* help screen short option */
    case 'h':
      fHelp = 1;
      break;

      /* lower case short option */
    case 'l':
      fToLower = 1;
      break;

      /* line numbers short option */
    case 'n':
      fLineNo = 1;
      break;

      /* replace character short option, capture characters to use in 
         operation */
    case 'r':
      fReplace = 1;
      replaceChars = optarg;
      break;

      /* upper case short option, set the flag */
    case 'u':
      fToUpper = 1;
      break;

      /* expand tab short option, get number of spaces to use instead of tabs */
    case 'x':
      fTabExpand = 1;

      /* get the number if given else use default number */
      if (optarg)
      {
        tabExpandNo = atoi(optarg);
      }
      else
      {
        tabExpandNo = SPACES;
      }
      break;

      /* unknown option */
    case '?':
      printf("invalid option\n");
      break;

      /* missing argument to an option */
    case ':':
      printf("missing argument for %C\n", optopt);
      break;

      /* non-option (file) */
    case 1:
      /* increment counter */
      nonOptions++;
      break;
    }
  }

  /* Allocate for exact amount of non-options */
  if (nonOptions != 0)
  {
    Files = malloc(sizeof(char *) * nonOptions);
  }
  /* reset optind to parse again */
  optind = 1;
}

/*****************************************************************************/
/*!
    \brief
      Gets the files based on name from command line.

    \param argc
      Number of arguments passed.

    \param argv
      Null terminated array of arguments to the command line.
      
*/
/*****************************************************************************/
static void Assign(int argc, char **argv)
{
  /* stores result of get opt */
  int store;

  /* counter to fill in array */
  int fileCount = 0;

  while (1)
  {
    int optionIndex = 0;

    /* struct defining long-options */
    static struct option LongOptions[] =
    {
      { "remove-blank-lines", no_argument,       &fRemoveBlank,  1 },
      { "delete",             required_argument, &fDelete,       1 },
      { "help",               no_argument,       &fHelp,         1 },
      { "tolower",            no_argument,       &fToLower,      1 },
      { "line-numbers",       no_argument,       &fLineNo,       1 },
      { "replace",            required_argument, &fReplace,      1 },
      { "toupper",            no_argument,       &fToUpper,      1 },
      { "expand-tabs",        optional_argument, &fTabExpand,    1 },
      { NULL,                 0,                 NULL,           0 }
    };

    /* get argument from command line */
    store = getopt_long(argc, argv, "-:bd:hlnr::ux:", LongOptions, 
                        &optionIndex);

    /* stop at end of command line */
    if (store == -1)
    {
      break;
    }
    /* only non options are important rest can be ignored */

    switch (store)
    {
      case 'b':
      {
        break;
      }
      case 'd':
      {
        break;
      }
      case 'h':
      {
        break;
      }
      case 'l':
      {
        break;
      }
      case 'n':
      {
        break;
      }
      case 'r':
      {
        break;
      }
      case 'u':
      {
        break;
      }
      case 'x':
      {
        break;
      }
        /* non-option encountered, store in array */
      case 1:
      {

        /* store file in array */
        Files[fileCount] = argv[optind - 1];

        /* increment counter to fill in next spot */
        fileCount++;
        break;
      }
    }
  }
}

/*****************************************************************************/
/*!
    \brief
      Capitalize a character if lowercase, anything else is ignored.
      
    \param letter
      Character being modified.

*/
/*****************************************************************************/
static void ToUpper(int *letter)
{
  /* if given a lowercase letter, capitalize it. */
  if ('a' <= *letter && *letter <= 'z')
  {
    *letter -= 32;
  }
  else
  {
    return;
  }
}

/*****************************************************************************/
/*!
    \brief
      Lowercase a character if capitalized, anything else is ignored.

    \param letter
      Character being modified.

*/
/*****************************************************************************/
static void ToLower(int *letter)
{
  /* if given a lowercase letter, capitalize it. */
  if ('A' <= *letter && *letter <= 'Z')
  {
    *letter += 32;
  }
  else
  {
    return;
  }
}

/*****************************************************************************/
/*!
    \brief
      Deletes character by simply jumping over it and will delete all of them.

    \param letter
      Letter getting 'removed'.

    \param input
      Pointer to file being filtered.
      
*/
/*****************************************************************************/
static void DeleteChars(int *letter, FILE *input)
{
  /* while the character is the one to be deleted, get the next from the
  * file */
  while (*letter == deleteChar)
  {
    *letter = fgetc(input);
  }
}

/*****************************************************************************/
/*!
    \brief
      Increase the tab spacing based on a number.

*/
/*****************************************************************************/
static void Expand(void)
{
  /* counter */
  int i;

  /* replace tab with a number of spaces, specified from commmand line */
  for (i = 0; i < tab_expand_num; i++)
  {
    putc(' ', stdout);
  }
}

/*****************************************************************************/
/*!
    \brief
      Filters a file based on flags given to it.

    \param input
      File being filtered.

*/
/*****************************************************************************/
static void Filter(FILE *input)
{
  /* current char */
  int curr = 0;

  /* previous char */
  int prev = 0;

  /* counter to print new lines */
  static int lineCount = 1;

  /* if flag is true, print the line number */
  if (fLineNo)
  {
    printf("%6i  ", lineCount);
    lineCount++;
  }

  /* while there are characters to print out */
  while (1)
  {
    /* set previous char to what was just printed */
    prev = curr;

    /* get next character form the file */
    curr = fgetc(input);

    /* if end of file */
    if (curr == EOF)
    {
      break;
    }
    /* Call delete function to delete the char */
    if (fDelete && curr == deleteChar)
    {
      DeleteChars(&curr, input);
    }
    /* Call replace function to replace */
    if (fReplace && curr == replaceChars[0])
    {
      curr = replaceChars[1];
    }
    /* Captilize the char */
    if (fToUpper)
    {
      ToUpper(&curr);
    }
    /* if lower case flag is set call lower case flag helper function */
    if (fToLower)
      ToLower(&curr);

    /* Expand the tab using default number */
    if (fTabExpand && (curr == TAB))
    {
      /* Am I a new line? */
      if (fLineNo && prev == NEWLINE)
      {
        printf("%6i  ", lineCount);
        lineCount++;
      }

      /* Expand the tabs */
      Expand();

      /* Keep Moving */
      continue;
    }

    /* You and you aren't characters... Time to move along */
    if (fRemoveBlank && curr == NEWLINE && prev == NEWLINE)
    {
      continue;
    }

    /* Oh, you were empty. Time to get moving again. */
    if (fLineNo && prev == NEWLINE)
    {
      printf("%6i  ", lineCount);
      lineCount++;
    }

    /* print out character to stdout */
    putc(curr, stdout);
  }

}

/*****************************************************************************/
/*!
    \brief
      Filters files that are given or stdin if no files are given.

    \param argc
      Arguments passed in command line.

    \param argv
      Arguments from command line.

    \return
      End the program when it is done.
*/
/*****************************************************************************/
int main(int argc, char **argv)
{
  /* In-put... is it something you can eat? */
  FILE *input;

  /* counter */
  int count;

  /* Parse all the files and give me a report! */
  Parse(argc, argv);

  /* help me... */
  if (fHelp)
  {
    /* S.O.S */
    HelpScreen();
    return 0;
  }

  /* Time to assign you a new partner */
  if (nonOptions != 0)
  {
    Assign(argc, argv);
  }
  /* WHERE ARE ALL THE FILES!? */
  else
  {
    /* hey you! what are you trying to give me */
    input = stdin;

    /* FILTER! */
    Filter(input);

    return 0;
  }

  /* How many filters are there? */
  for (count = 0; count < nonOptions; count++)
  {
    /* time to read and write people */
    input = fopen(Files[i], "rt");

    /* Are you or are you not a point? */
    if (!input)
    {
      continue;
    }
    /* time to filter out all the bad things */
    Filter(input);

    /* Goodbye */
    fclose(input);
  }

  /* Free me! */
  free(Files);


  return 0;
}
