#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {
   IN, OUT, ID, PARENIZQ, PARENDERECHO, PUNTO, COMA, ASIGNACION, FDT, DNI, NOMBRE, HABITACION, GUIONBAJO, ASIGNHABITACION, INGPACIENTE
} TOKEN;

// --SCANNER --
TOKEN esReservada(char[]);
TOKEN queReservadaEs(char[]);
TOKEN scanner(FILE*);
void mostrarTabla();
void limpiarBuffer(char []);
void sacarUltimoCaracter(char[]);
int pertenece(int[], int);
void Match(TOKEN, FILE*);
void errorLexico();


char buffer[32];
char auxiliar[32];
int estadosFinales[] = {2,4,7,15,16,17,18,20,21,'\0'};
int indiceTS = 4;


// --PARSER--
void errorSintactico();
void objetivo(FILE*);
void programa(FILE*);
void ListaSentencias(FILE*);
void Sentencia(FILE*);
void Cuarto(FILE*);
void Expresion(FILE*);
void Paciente(FILE*);
void Documento(FILE*);

// -- TABLA DE SIMBOLOS --
char* TS[20][2] = {{"in" , "IN"},
                   {"out", "OUT"},
                   {"ingresarPaciente", "INGPACIENTE"},
                   {"asignarHabitacion", "ASIGNHABITACION"},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""},
                   {"", ""}};

// -- TABA DE AFD --
int tablaAFD[23][11] = {{1 ,5 ,16,17,18,22,19,22,21,0 ,22},
                        {1 ,2 ,2 ,2 ,2 ,2 ,2 ,3 ,2 ,2 ,2 },
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {3 ,4 ,4 ,4 ,4 ,4 ,4 ,4 ,4 ,4 ,4 },
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {22,6 ,22,22,22,22,22,22,22,22,22},
                        {22,7 ,22,22,22,8 ,22,22,22,22,22},
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {22,9 ,22,22,22,22,22,22,22,22,22},
                        {22,10,22,22,22,22,22,22,22,22,22},
                        {22,11,22,22,22,22,22,22,22,22,22},
                        {22,22,22,22,22,12,22,22,22,22,22},
                        {22,13,22,22,22,22,22,22,22,22,22},
                        {22,14,22,22,22,22,22,22,22,22,22},
                        {22,15,22,22,22,22,22,22,22,22,22},
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {22,22,22,22,22,22,20,22,22,22,22},
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {22,22,22,22,22,22,22,22,22,22,22},
                        {22,22,22,22,22,22,22,22,22,22,22}};
// link de explicacion tabla: https://docs.google.com/document/d/1-50RE-FaFFi9SNszNAd_tr_MRUCRn_MPpspRDcTgZUw/edit


//-----------------------------------------------------------------------------------------------------------------//
                                     // ----------------- MAIN -------------------- //
//-----------------------------------------------------------------------------------------------------------------//
int main(){
   int i=0;
   char caracter;
   char nombre[20];

// Ingresar el archivo
   printf("Ingrese el archivo de texto a compilar: ");
   scanf("%s",nombre);
   FILE *archivoTxt = fopen( nombre, "r" );

// abre el archivo: 
   archivoTxt = fopen( nombre, "r" );
   printf( "Archivo: %s -> ", nombre );
    if( archivoTxt ) printf( "Existe (ABIERTO)\n\n" );
      else{
         printf( "Error (NO ABIERTO)\n\n" );
         return 1;
      }

      objetivo(archivoTxt);

      fclose(archivoTxt); 
  return 0;
}


//-----------------------------------------------------------------------------------------------------------------//
                                     // ----------------- SCANNER -------------------- //
//-----------------------------------------------------------------------------------------------------------------//

TOKEN scanner(FILE *archivo){
   
   limpiarBuffer(buffer);
   int a,i = 0;
   char caracter;
   int estado = 0;
   int columna = 0;
   int estadoRechazo = 22;

   // Reconoce el fin de texto
   if(feof(archivo)) {estado = 21;};

   while(!pertenece(estadosFinales,estado)) {
      
      caracter = fgetc(archivo);

      if(!isspace(caracter)){
         buffer[i] = caracter;
         i++; 
      };

      if(isalpha(caracter)){
        columna = 0;
      } else {

      if(isdigit(caracter)){
        columna = 1;
      } else {
      
      if(caracter == '('){
         columna = 2;
      } else {

      if(caracter == ')'){
         columna = 3;
      } else {

      if(caracter == ','){
         columna = 4;
      }   else {

      if(caracter == '.'){
         columna = 5;
      } else {
      
      if(caracter == ':'){
         columna = 6;
      } else {

      if(caracter == '_'){
         columna = 7;
      } else {

      if(isspace(caracter)){
         columna = 9;
      } else {
         columna = 10;

      }}}}}}}}};
      estado = tablaAFD[estado][columna];

      if(estado == estadoRechazo){
        break;
      };
   }


   switch (estado){

         case 2:
            
            if(!isspace(caracter)){sacarUltimoCaracter(buffer);ungetc(caracter, archivo);};
            return esReservada(buffer);
               
         case 4:
            sacarUltimoCaracter(buffer);
            if(!isspace(caracter)){ungetc(caracter, archivo);};
            return NOMBRE;
         
         case 7:
            return HABITACION;

         case 15:
            return DNI;
         
         case 16:
            return PARENIZQ;
         
         case 17:
            return PARENDERECHO;
         
         case 18:
            return COMA;
         
         case 20:
            return ASIGNACION;
         
         case 21:
            return FDT;

         default:
            errorLexico();
      };
   
};

int pertenece(int cadena[], int numero){
   
	int i = 0;  
	while(cadena[i] != '\0'){
      
		if (cadena[i] == numero) { return 1;
		}
		else { i++; }
	}
	 return 0; 
};

TOKEN esReservada(char palabra[]){
   int i = 0;
   while(strcmp( TS[i][0] , "")  != 0){
      if(strcmp( TS[i][0] , palabra)  == 0){
         return queReservadaEs(TS[i][1]);
      }
      i++;
   };

   if(strlen(palabra) <= 32){
       return ID;
   } else
   {
      errorSintactico();
   }
   
};

TOKEN queReservadaEs(char reservada[]){
   
   if(strcmp(reservada, "IN") == 0){
      return IN;
   } else {
   if(strcmp(reservada, "OUT") == 0){ 
      return OUT; 
   } else {
   if(strcmp(reservada, "ASIGNHABITACION") == 0){
      return ASIGNHABITACION;
   } else{
   if(strcmp(reservada, "INGPACIENTE") == 0){
      return INGPACIENTE;
   } else { 
      return ID;
   }}}};

};

void mostrarTabla() {
   int i = 0;
   while(i < 19){
      printf("%s = %s \n", TS[i][0], TS[i][1]);
      i++;
}};

void limpiarBuffer(char buffer[]){
   memset(buffer,0,31);
};

void sacarUltimoCaracter(char buffer[]){
   strcpy(auxiliar, buffer);
   limpiarBuffer(buffer);

   for(int a = 0; a < (strlen(auxiliar)-1); a++){
      
      buffer[a] = auxiliar[a];
   }
   };
   

//-----------------------------------------------------------------------------------------------------------------//
                                     // ----------------- PARSER -------------------- //
//-----------------------------------------------------------------------------------------------------------------//

void Match(TOKEN tok, FILE* archivo){
   TOKEN tokenProximo = scanner(archivo);
   if(tok != tokenProximo){
      errorSintactico();
   };
};

void objetivo(FILE* archivoTxt){
    programa(archivoTxt);
    Match(FDT,archivoTxt);
    printf("La compilacion se realizo correctamente.");
}; 

void programa(FILE* archivoTxt){
   Match(IN,archivoTxt);
   ListaSentencias(archivoTxt);   
};

void ListaSentencias (FILE* archivoTxt) {
  Sentencia(archivoTxt); 
  while(1) { 
    switch(scanner(archivoTxt)) {
	case ASIGNHABITACION:
      Match(PARENIZQ, archivoTxt);
      Documento(archivoTxt); 
      Match(COMA, archivoTxt); 
      Cuarto(archivoTxt); 
      Match(PARENDERECHO, archivoTxt);
      break;
	case INGPACIENTE: 
      Match(PARENIZQ, archivoTxt); 
      Paciente(archivoTxt); 
      Match(COMA, archivoTxt); 
      Documento(archivoTxt); 
      Match(PARENDERECHO, archivoTxt);
      break;
	case ID: 
      Match(ASIGNACION, archivoTxt); Expresion(archivoTxt);
      break;
   case OUT:
      return;

	default:
      errorSintactico();
      return;
    } 
  } 
}

void Sentencia(FILE* archivoTxt){
 TOKEN tok = scanner(archivoTxt);
 switch(tok){
  case ID: 
   Match(ASIGNACION, archivoTxt); Expresion(archivoTxt);
   break;
  case ASIGNHABITACION:
   Match(PARENIZQ, archivoTxt); Documento(archivoTxt); Match(COMA, archivoTxt); Cuarto(archivoTxt); Match(PARENDERECHO, archivoTxt);
   break;
  case INGPACIENTE:
   Match(PARENIZQ, archivoTxt); Paciente(archivoTxt); Match(COMA, archivoTxt); Documento(archivoTxt); Match(PARENDERECHO, archivoTxt);
   break;

  default:
  errorSintactico();
    break;
 }
}

void Expresion(FILE* archivoTxt){
   switch(scanner(archivoTxt)){
      case ID:
         break;
      case DNI:
         break;
      case HABITACION:
         break;
      case NOMBRE:
         break;

      default:
         errorSintactico();
   }
};

void Documento(FILE* archivoTxt){
   switch(scanner(archivoTxt)){
      case ID:
         break;
      case DNI:
         break;
         
      default:
         errorSintactico();
   } 
};

void Paciente(FILE* archivoTxt){
   switch(scanner(archivoTxt)){
      case ID:
         break;
      case NOMBRE:
         break;
         
      default:
         errorSintactico();
   }
};

void Cuarto(FILE* archivoTxt){
   switch(scanner(archivoTxt)){
      case ID:
         break;
      case HABITACION:
         break;

      default:
         errorSintactico();    
   }  
};

void errorSintactico(){
   printf("Error sintactico. La sentencia erronea fue: '%s' \n", buffer);
   abort();
};

void errorLexico(){
   printf("Error lexico. El lexema erroneo fue: '%s' \n", buffer);
};