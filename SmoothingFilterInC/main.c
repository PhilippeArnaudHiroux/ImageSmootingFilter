//Philippe-Arnaud Hiroux

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __DEBUG

#define BLUE 0
#define GREEN 1
#define RED 2

#define HOEK 4
#define RAND 6
#define MIDDEN 9

//BMP INPUTS
//#define BMPINPUTFILE "foto_1.bmp" //Deze foto werkt (deelbaar door 4)
//#define BMPINPUTFILE "foto_2.bmp" //Deze foto werkt (deelbaar door 4)
//#define BMPINPUTFILE "foto_3.bmp" //Deze foto werkt niet (nier deelbaar door 4)
#define BMPINPUTFILE "../foto_4.bmp" //Deze foto werkt (deelbaar door 4)


//BMP OUTPUTS
#define BMPOUTPUTFILE "../output.bmp"
#define EXTRA "extra.bmp"

int controleBreendteEnHoogte(int b, int h);
void kleurenSamenvoegen(char* output, char* outputBlauw, char* outputGroen, char* outputRood, int afbeeldingGroote, char* BMPheader);
//void kleur(int imageSize, char* BMPheader, char* inputPixels);

int main()
{
    #ifdef __DEBUG
        printf("DEBUG info: BMP transformer\n");
    #endif

    FILE* inputFilePointer = fopen(BMPINPUTFILE, "rb"); //maak een file pointer naar de afbeelding
    if(inputFilePointer == NULL){ //Test of het open van de file gelukt is!
        printf("Er ging iets mis toen tijdens het openen van %s\n", BMPINPUTFILE); //Print de tekst uit
        exit(EXIT_FAILURE); //Ga uit het programma
    }

    #ifdef __DEBUG
        printf("DEBUG info: Opening File OK: %s\n", BMPINPUTFILE); //Print de tekst uit
    #endif

    unsigned char bmpHeader[54]; // voorzie een array van 54-bytes voor de BMP Header
    fread(bmpHeader, sizeof(unsigned char), 54, inputFilePointer); // lees de 54-byte header

/*Bereken de groote van de afbeelding*/
    int breedte = *(int*)&bmpHeader[18]; //Haal de breedte uit de header
    int hoogte = *(int*)&bmpHeader[22]; //Haal de hoogte uit de header
    int imageSize = 3 * breedte * hoogte;

    #ifdef __DEBUG
        printf("DEBUG info: breedte = %d\n", breedte); //Print de breedte uit
        printf("DEBUG info: hoogte = %d\n", hoogte); //Print de hoogte uit
    #endif

    int controle =controleBreendteEnHoogte(breedte,hoogte);
    if(controle != 0){
        return -1;
    }

/*Alloceer geheugen*/
    unsigned char* inputPixels = (unsigned char *) calloc(imageSize, sizeof(unsigned char));
    unsigned char* blauwPixels = (unsigned char *) calloc(imageSize/3, sizeof(unsigned char));
    unsigned char* groenPixels = (unsigned char *) calloc(imageSize/3, sizeof(unsigned char));
    unsigned char* roodPixels = (unsigned char *) calloc(imageSize/3, sizeof(unsigned char));
    unsigned char* outputPixels = (unsigned char *) calloc(imageSize, sizeof(unsigned char));
    unsigned char* outputBlauwPixels = (unsigned char *) calloc(imageSize/3, sizeof(unsigned char));
    unsigned char* outputGroenPixels = (unsigned char *) calloc(imageSize/3, sizeof(unsigned char));
    unsigned char* outputRoodPixels = (unsigned char *) calloc(imageSize/3, sizeof(unsigned char));

/*Lees alle pixels (de rest van de file*/
    fread(inputPixels, sizeof(unsigned char), imageSize, inputFilePointer);

     //kleur(imageSize, bmpHeader, inputPixels); //Voor de extra kleur funcite uit

/*Maak j en a aan als integers en zet ze op 0*/
    int j = 0;
    int a = 0;

/*Hier worden de blauwe, de groene en de rode kleur van en kaar gescheiden*/
    for(int i=0; i<breedte*hoogte; i++){
        blauwPixels[i] = inputPixels[j];
        j++;
        groenPixels[i] = inputPixels[j];
        j++;
        roodPixels[i] = inputPixels[j];
        j++;
    }

/*Hier wordt de bewerking van de blauwe kleur uitgevoerd*/
    for(int i=0; i<breedte; i++){ //Bovenste rij
        if(i==0){ //Links boven
            outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a+1] + blauwPixels[a+breedte] + blauwPixels[a+breedte+1])/HOEK;
            a++;
        }
        if(i>0 && i<breedte-1){ //Midden boven
            outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a-1] + blauwPixels[a+1] + blauwPixels[a+breedte-1] + blauwPixels[a+breedte] + blauwPixels[a+breedte+1])/RAND;
            a++;
        }
        if(i==breedte-1){ //Rechts boven
            outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a-1] + blauwPixels[a+breedte-1] + blauwPixels[a+breedte])/HOEK;
            a++;
        }
    }
    for(int i=0; i<hoogte-2; i++){
        for(int j=0; j<breedte; j++){
            if(j==0){ //Linker rand
                outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a-breedte] + blauwPixels[a-breedte+1] + blauwPixels[a+1] + blauwPixels[a+breedte] + blauwPixels[a+breedte+1])/RAND;
                a++;
            }
            if(j>0 && j<breedte-1){ //Midden
                outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a-1] + blauwPixels[a-breedte-1] + blauwPixels[a-breedte] + blauwPixels[a-breedte+1] + blauwPixels[a+1] + blauwPixels[a+breedte-1] + blauwPixels[a+breedte] + blauwPixels[a+breedte+1])/MIDDEN;
                a++;
            }
            if(j==breedte-1){ //Rechter rand
                outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a-1] + blauwPixels[a-breedte-1] + blauwPixels[a-breedte] + blauwPixels[a+breedte-1] + blauwPixels[a+breedte])/RAND;
                a++;
            }
        }
    }
    for(int i=0; i<breedte; i++){
        if(i==0){ //Links onder
            outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a-breedte] + blauwPixels[a-breedte+1] + blauwPixels[a+1])/HOEK;
            a++;
        }
        if(i>0 && i<breedte-1){ //Midden onder
            outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a-1] + blauwPixels[a-breedte-1] + blauwPixels[a-breedte] + blauwPixels[a-breedte+1] + blauwPixels[a+1])/RAND;
            a++;
        }
        if(i==breedte-1){ //Links onder
            outputBlauwPixels[a] = (blauwPixels[a] + blauwPixels[a-1] + blauwPixels[a-breedte] + blauwPixels[a-breedte-1])/HOEK;
            a++;
        }
    }

/*Hier wordt de bewerking van de groene kleur uitgevoerd*/
    a =0;

    for(int i=0; i<breedte; i++){ //Bovenste rij
        if(i==0){ //Links boven
            outputGroenPixels[a] = (groenPixels[a] + groenPixels[a+1] + groenPixels[a+breedte] + groenPixels[a+breedte+1])/HOEK;
            a++;
        }
        if(i>0 && i<breedte-1){ //Midden boven
            outputGroenPixels[a] = (groenPixels[a] + groenPixels[a-1] + groenPixels[a+1] + groenPixels[a+breedte-1] + groenPixels[a+breedte] + groenPixels[a+breedte+1])/RAND;
            a++;
        }
        if(i==breedte-1){ //Rechts boven
            outputGroenPixels[a] = (groenPixels[a] + groenPixels[a-1] + groenPixels[a+breedte-1] + groenPixels[a+breedte])/HOEK;
            a++;
        }
    }
    for(int i=0; i<hoogte-2; i++){
        for(int j=0; j<breedte; j++){
            if(j==0){ //Linker rand
                outputGroenPixels[a] = (groenPixels[a] + groenPixels[a-breedte] + groenPixels[a-breedte+1] + groenPixels[a+1] + groenPixels[a+breedte] + groenPixels[a+breedte+1])/RAND;
                a++;
            }
            if(j>0 && j<breedte-1){ //Midden
                outputGroenPixels[a] = (groenPixels[a] + groenPixels[a-1] + groenPixels[a-breedte-1] + groenPixels[a-breedte] + groenPixels[a-breedte+1] + groenPixels[a+1] + groenPixels[a+breedte-1] + groenPixels[a+breedte] + groenPixels[a+breedte+1])/MIDDEN;
                a++;
            }
            if(j==breedte-1){ //Rechter rand
                outputGroenPixels[a] = (groenPixels[a] + groenPixels[a-1] + groenPixels[a-breedte-1] + groenPixels[a-breedte] + groenPixels[a+breedte-1] + groenPixels[a+breedte])/RAND;
                a++;
            }
        }
    }
    for(int i=0; i<breedte; i++){
        if(i==0){ //Links onder
            outputGroenPixels[a] = (groenPixels[a] + groenPixels[a-breedte] + groenPixels[a-breedte+1] + groenPixels[a+1])/HOEK;
            a++;
        }
        if(i>0 && i<breedte-1){ //Midden onder
            outputGroenPixels[a] = (groenPixels[a] + groenPixels[a-1] + groenPixels[a-breedte-1] + groenPixels[a-breedte] + groenPixels[a-breedte+1] + groenPixels[a+1])/RAND;
            a++;
        }
        if(i==breedte-1){ //Links onder
            outputGroenPixels[a] = (groenPixels[a] + groenPixels[a-1] + groenPixels[a-breedte] + groenPixels[a-breedte-1])/HOEK;
            a++;
        }
    }


/*Hier wordt de bewerking van de rode kleur uitgevoerd*/
    a =0;

    for(int i=0; i<breedte; i++){ //Bovenste rij
        if(i==0){ //Links boven
            outputRoodPixels[a] = (roodPixels[a] + roodPixels[a+1] + roodPixels[a+breedte] + roodPixels[a+breedte+1])/HOEK;
            a++;
        }
        if(i>0 && i<breedte-1){ //Midden boven
            outputRoodPixels[a] = (roodPixels[a] + roodPixels[a-1] + roodPixels[a+1] + roodPixels[a+breedte-1] + roodPixels[a+breedte] + roodPixels[a+breedte+1])/RAND;
            a++;
        }
        if(i==breedte-1){ //Rechts boven
            outputRoodPixels[a] = (roodPixels[a] + roodPixels[a-1] + roodPixels[a+breedte-1] + roodPixels[a+breedte])/HOEK;
            a++;
        }
    }
    for(int i=0; i<hoogte-2; i++){
        for(int j=0; j<breedte; j++){
            if(j==0){ //Linker rand
                outputRoodPixels[a] = (roodPixels[a] + roodPixels[a-breedte] + roodPixels[a-breedte+1] + roodPixels[a+1] + roodPixels[a+breedte] + roodPixels[a+breedte+1])/RAND;
                a++;
            }
            if(j>0 && j<breedte-1){ //Midden
                outputRoodPixels[a] = (roodPixels[a] + roodPixels[a-1] + roodPixels[a-breedte-1] + roodPixels[a-breedte] + roodPixels[a-breedte+1] + roodPixels[a+1] + roodPixels[a+breedte-1] + roodPixels[a+breedte] + roodPixels[a+breedte+1])/MIDDEN;
                a++;
            }
            if(j==breedte-1){ //Rechter rand
                outputRoodPixels[a] = (roodPixels[a] + roodPixels[a-1] + roodPixels[a-breedte-1] + roodPixels[a-breedte] + roodPixels[a+breedte-1] + roodPixels[a+breedte])/RAND;
                a++;
            }
        }
    }
    for(int i=0; i<breedte; i++){
        if(i==0){ //Links onder
            outputRoodPixels[a] = (roodPixels[a] + roodPixels[a-breedte] + roodPixels[a-breedte+1] + roodPixels[a+1])/HOEK;
            a++;
        }
        if(i>0 && i<breedte-1){ //Midden onder
            outputRoodPixels[a] = (roodPixels[a] + roodPixels[a-1] + roodPixels[a-breedte-1] + roodPixels[a-breedte] + roodPixels[a-breedte+1] + roodPixels[a+1])/RAND;
            a++;
        }
        if(i==breedte-1){ //Links onder
            outputRoodPixels[a] = (roodPixels[a] + roodPixels[a-1] + roodPixels[a-breedte] + roodPixels[a-breedte-1])/HOEK;
            a++;
        }
    }
    kleurenSamenvoegen(outputPixels, outputBlauwPixels, outputGroenPixels, outputRoodPixels, imageSize, bmpHeader);

    /*Hier worden de files gesloten*/
    fclose(inputFilePointer);

    /*Hier wordt het geheugen terug vrijgegeven*/
    free(inputPixels);
    free(blauwPixels);
    free(groenPixels);
    free(roodPixels);
    free(outputBlauwPixels);
    free(outputGroenPixels);
    free(outputRoodPixels);
    free(outputPixels);

    return 0;
}

int controleBreendteEnHoogte(int b, int h)
{
    if(b % 4 != 0){
        printf("DEBUG info: De breedte van de foto is geen veelvoud van 4!!!\n");
        return -1;
    }
    else if(h % 4 != 0){ //Als de hoogte niet deelbaar is door 4
        printf("DEBUG info: De hoogte van de foto is geen veelvoud van 4!!!\n");
        return -1;
    }
    else{
        printf("DEBUG info: De foto is goed\n");
        return;
    }
}

void kleurenSamenvoegen(char* output, char* outputBlauw, char* outputGroen, char* outputRood, int afbeeldingGroote, char* BMPheader)
{
    FILE* outputFilePointer = fopen(BMPOUTPUTFILE, "wb"); //Maak een file pointer naar de nieuwe afbeelding
    if(outputFilePointer == NULL){ //Test of het open van de file gelukt is!
        printf("Er ging iets mis toen tijdens het openen van %s\n", BMPOUTPUTFILE); //Print de tekst uit
        exit(EXIT_FAILURE); //Ga uit het programma
    }

    #ifdef __DEBUG
        printf("DEBUG info: Opening File OK: %s\n", BMPOUTPUTFILE); //Print de tekst uit
    #endif
    int j = 0;

    for(int i=0; i<afbeeldingGroote/3; i++)
    {
        output[j] = outputBlauw[i];
        j++;
        output[j] = outputGroen[i];
        j++;
        output[j] = outputRood[i];
        j++;
    }

    fwrite(BMPheader, 1, 54, outputFilePointer);
    fwrite(output, 1, afbeeldingGroote, outputFilePointer);
    fclose(outputFilePointer);
}
/*
void kleur(int imageSize, char* BMPheader, char* inputPixels)
{
    int kleurWaarde = 0; //Maak een integer aan me de naam kleurWaarde

    FILE* extras = fopen(EXTRA, "wb"); //Maak een file pointer naat de extra file
    unsigned char* extraOpdracht = (unsigned char *) calloc(imageSize, sizeof(unsigned char)); // allocate een array voor alle pixels voor de extra opdracht
    if(extras == NULL){ //Test of het open van de file gelukt is!
        printf("Er ging iets mis toen tijdens het openen van %s\n", BMPINPUTFILE);
        exit(EXIT_FAILURE);
    }

    //Hier ga je kiezen wat je wilt doen
    do{
        printf("Als je 1 kleur in de foto wil behouden duw -> 0\nAls je 1 kleur in je foto wil wegfilteren duw -> 1\n>>> ");
        scanf("%d", &kleurWaarde);
    }while(kleurWaarde < 0 || kleurWaarde > 1);

    if(kleurWaarde == 0){
        do{
            printf("Voor een blauwe foto duw -> 0\nVoor een groene foto duw -> 1\nVoor een rode foto duw -> 2\n>>> ");
            scanf("%d", &kleurWaarde);
        }while(kleurWaarde<0 || kleurWaarde>2);

        //Voer de gekozen kleur uit
        for(int i = 0; i < imageSize; i++){
            if( i % 3 == kleurWaarde){
                extraOpdracht[i] = inputPixels[i];
            }
            else{
                extraOpdracht[i] = 0;
            }
        }
    }
    else{
        do{
            printf("Voor blauw weg te filteren duw -> 0\nVoor groen weg te filteren duw -> 1\nVoor rood weg te filteren duw -> 2\n>>> ");
            scanf("%d", &kleurWaarde);
        }while(kleurWaarde<0 || kleurWaarde>2);

        //Voer de gekozen kleur uit
        for(int i = 0; i < imageSize; i++){
            if( i % 3 != kleurWaarde){
                extraOpdracht[i] = inputPixels[i];
            }
            else{
                extraOpdracht[i] = 0;
            }
        }
    }

    fwrite(BMPheader, 1, 54, extras); //Schrijf BMPheader naar extras
    fwrite(extraOpdracht, 1, imageSize, extras); //Schrijf extraOpdracht naar extras
    fclose(extras); //Close extras FILE
    free(extraOpdracht); //Geef extraOpdracht vrij
}*/
