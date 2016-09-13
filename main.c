#include <stdio.h>
#include <stdlib.h>
#define CON_MSJ 1
#define ARCHIVO "datos"

/*COSAS IMPORTANTES DE ESTE PROGRAMA:
    -utilizacion correcta de scanf y de gets.
    -modos de apertura de los archivos.
    -utilizacion de punteros.
    -referencia a estructuras que fueron pasadas con aritmetica de punteros.
    -utilizacion de funcines de archivo como fseek y ftell.
    -SEEK_SET, SEET_SUR, SEEK_END.
    -Particularidad del lenguaje de posicionar el puntero al archivo desp del fwrite.
    -utilizacion del fread y del fwrite.
*/
typedef struct
{
    char apyn[30];
    int dni;
    char sexo;
    float sueldo;
}t_persona;

int abrirArchivo(FILE** fp, char* nombreArchivo, char* modo, int MSJ);
int cargarArchivo(FILE** fp);
void mostrarArchivo(FILE** fp, t_persona* aux);
int cantidadDeRegistros(FILE** fp);


int main()
{
    FILE * fpDatos;
    t_persona reg;

    if(!abrirArchivo(&fpDatos, ARCHIVO, "wb", CON_MSJ))
        return 1;
    cargarArchivo(&fpDatos);
    fclose(fpDatos);


    if(!abrirArchivo(&fpDatos, ARCHIVO, "rb", CON_MSJ))
        return 1;
    mostrarArchivo(&fpDatos, &reg);
    fclose(fpDatos);

//PROCESO DE ACTUALIZACION
    if(!abrirArchivo(&fpDatos, ARCHIVO, "rb+", CON_MSJ))
        return 1;
    fread(&reg, sizeof(t_persona), 1, fpDatos);
    while(!feof(fpDatos))
    {
        if(reg.sexo == 'F')
        {
            reg.sueldo *= 1.15;
            fseek(fpDatos, -1L * sizeof(t_persona), SEEK_CUR);
            fwrite(&reg, sizeof(t_persona), 1, fpDatos);
            fseek(fpDatos, 0L, SEEK_CUR); //HAY QUE HACER ESTO PORQUE EL LENGUAJE ASI LO EXIJE
        }
        fread(&reg, sizeof(t_persona), 1, fpDatos);
    }
    fclose(fpDatos);

    puts("\n\nACTUALIZAMOS EL ARCHIVO. . . \n");
    system("pause");

    if(!abrirArchivo(&fpDatos, ARCHIVO, "rb", CON_MSJ))
        return 1;
    mostrarArchivo(&fpDatos, &reg);
    printf("\nEl archivo tiene .. %d .. registros.\n", cantidadDeRegistros(&fpDatos));
    fclose(fpDatos);


    return 0;
}

int abrirArchivo(FILE** fp, char* nombreArchivo, char* modo, int MSJ)
{
    *fp = fopen(nombreArchivo, modo);
    if(!fp)
    {
        if(MSJ)
            printf("\nError al intentar abrir / crear el archivo. Fin de programa.\n");
        return 0;
    }
    return 1;
}

int cargarArchivo(FILE** fp)
{
    t_persona varDeCarga;
    char eleccion;
    int cantElememtosCargados = 0;

    printf("\nCARGA DE DATOS EN EL ARCHIVO: \n");
    printf("\nPara cargar datos al archivo ingrese 's' o 'n'.");
    puts("\nDesea cargar datos al achivo???");
    fflush(stdin);
    scanf("%c", &eleccion);

    while(eleccion == 's')
    {
        puts("\nIngrese el DNI: ");
        scanf("%d", &varDeCarga.dni);

        puts("\nIngrese el nombre y el apellido: ");
        fflush(stdin);
        scanf("%[^\n]", varDeCarga.apyn);//gets(varDeCarga.apyn);

        puts("\nIngrese el sexo, M-Masculino, F-Femenino.: ");
        fflush(stdin);
        scanf("%c", &varDeCarga.sexo);

        puts("\nIngrese el sueldo: ");
        scanf("%f", &varDeCarga.sueldo);

        puts("\nDesea cargar datos al achivo???");
        fflush(stdin);
        scanf("%c", &eleccion);


        fwrite(&varDeCarga, sizeof(t_persona), 1, *fp);
        fseek(*fp, 0L, SEEK_CUR);
        cantElememtosCargados++;
    }

    puts("\nFIN DE CARGA DE DATOS.");
    return cantElememtosCargados;
}

void mostrarArchivo(FILE** fp, t_persona* aux)
{
    printf("\nMostramos el ARCHIVO: ");
    printf("\n---------------------\n\n");
    fread(aux, sizeof(t_persona), 1, *fp);
    while(!feof(*fp))
    {
        printf("%-30s\t%8d\t%c\t%.2f\n",
               aux->apyn,
               aux->dni,
               aux->sexo,
               aux->sueldo);
        fread(aux, sizeof(t_persona), 1, *fp);
    }
}

int cantidadDeRegistros(FILE** fp) //DETERMINA LA CANTIDAD DE REGISTROS, al terminar nos posiciona donde estabamos.
{
    long posActual = ftell(*fp);//ftell devuelve la cant. de bytes desde el comienzo hasta donde estamos parados con el stream.
    int cantRegistros;

    fseek(*fp, 0L, SEEK_END);//VAMOS AL FINAL DEL ARCHIVO.
    cantRegistros = ftell(*fp) / sizeof(t_persona);
    fseek(*fp, posActual, SEEK_SET);//nos volvemos a parar donde estaba el stream.
    return cantRegistros;
}
