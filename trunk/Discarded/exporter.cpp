#include <stdio.h>
#include <string.h>

int ident = 0;
FILE *logf;

void space()
{
    for (int i=0; i<ident; i++) printf (" ");
}

void exporter (char *inf, FILE *ins, FILE *outs)
{
    int linecount = 0;
    char line[256], buf1[40], buf2[40];

    while (!feof(ins))
    {
        memset (line, 0, 256);
        memset (buf1, 0, 40);
        memset (buf2, 0, 40);
        fgets (line, 255, ins);
        linecount++;
        sscanf (line, "%s %s", buf1, buf2);

        if (!stricmp(buf1, "//%%new"))
        {
            ident+=4;
            space();
            printf ("New file: %s\n", buf2);
            FILE *ns = fopen (buf2, "w");
            if (ns == NULL)
            {
                space();
                printf ("File creation error\n");
            }

            fclose (ns);
            ident-=4;
            continue;
        }
        else if (!stricmp(buf1, "//%%end"))
        {
            fprintf (logf, "%-20s %d\n", inf, linecount);
            fclose(outs);
            return;
        }
        else if (!stricmp(buf1, "//%%export"))
        {
            ident+=4;
            space();
            printf ("Appending file: %s\n", buf2);
            FILE *ns = fopen (buf2, "a");
            if (ns == NULL)
            {
                space();
                printf ("File appending error.\n");
            }
            fprintf (logf, "%-20s %d\n", inf, linecount);
            linecount = 0;
            exporter (buf2, ins, ns);
            ident-=4;
            continue;
        }

        fputs (line, outs);
    }

    fprintf (logf, "%-20s %d\n", inf, linecount);
    fclose (outs);
}

int main (int argc, char **argv)
{
    if (argc != 3)
    {
        printf ("\nUsage: exporter inputfile outputfile(overwritten)\n");
        return (1);
    }

    FILE *ins  = fopen (argv[1], "r");
    FILE *outs = fopen (argv[2], "w");
          logf = fopen ("exporter.log", "w");

    if (ins == NULL || outs == NULL || logf == NULL)
    {
        printf ("\nFile creation/reading error\n");
        return (1);
    }

    exporter (argv[2], ins, outs);
    fclose (ins);

    return (0);
}

