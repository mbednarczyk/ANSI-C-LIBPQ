#include <stdlib.h>
#include <libpq-fe.h>
#include <string.h>
#include <stdio.h>
void doSQL(PGconn *conn, char *command)
{
  PGresult *result;

  printf("%s\n", command);

  result = PQexec(conn, command);
  printf("status is     : %s\n", PQresStatus(PQresultStatus(result)));
  printf("#rows affected: %s\n", PQcmdTuples(result));
  //WYSWIETLANIE BLEDOW!!!
  printf("result message: %s\n", PQresultErrorMessage(result));

  switch(PQresultStatus(result)) {
  case PGRES_TUPLES_OK:
    {
      int n = 0, m = 0;
      int nrows   = PQntuples(result);
      int nfields = PQnfields(result);
      printf("number of rows returned   = %d\n", nrows);
      printf("number of fields returned = %d\n", nfields);
      for(m = 0; m < nrows; m++) {
	for(n = 0; n < nfields; n++)
	  printf(" %s = %s", PQfname(result, n),PQgetvalue(result,m,n));
	printf("\n");
      }
    }
  }
  PQclear(result);
}

int main(){

int i=1;
int z=1;
int y, nowe_id;
PGconn *conn;
char nazwatabeli[20];
char zapytanie[250];
int value, wiek, pensja;
char kasa[20];
char imie[20], nazwisko[35], data_ur[10];
char  odp;


while(i!=0){
printf("\n---------------------------------\n");
		printf("OPERACJE NA BAZIE DANYCH");
		printf("\n---------------------------------\n");
 printf("Zakonczenie pracy = 0\nPolacz sie z baza = 1\nStworz tabele = 2\nUsun tabele = 3\nStworz wyzwalacz = 4\nUsun wyzwalacz = 5\nDodaj rekord do tabeli = 6\nModyfikuj rekord = 7\nUsun rekord = 8\nWyswietl zawartosc tabeli = 9\nWyszukiwanie w tabeli = 10\n");
       printf("\n");
       printf("Twoj wybor: ");
        scanf("%i",&i);
        printf("\n");
		z=1;
        switch(i){

        case 1:
 conn = PQconnectdb("host=localhost port=5432 dbname=aplikacje user=USER password=PASSWD");
  if(PQstatus(conn) == CONNECTION_OK) {
  printf("---------------------------------\n");
    printf("POLACZONO DO BAZY DANYCH\n");
	printf("---------------------------------");
	printf("\n");
	printf("\n");
		printf("\n");
		printf("\n");printf("\n");
	}
	 else{
    printf("\nPolaczenie nie udane! Koniec pracy programu\n: %s\n", PQerrorMessage(conn));
	i=0;
	printf("\n");
		printf("\n");
		printf("\n");printf("\n");
	}
	
	
        break;

        case 2:
		printf("\n---------------------------------\n");
		printf("TWORZENIE TABELI OSOBA");
		printf("\n---------------------------------\n");
		doSQL(conn, "DROP TABLE osoba");
		 doSQL(conn, "CREATE TABLE osoba(id_osoba INTEGER PRIMARY KEY, imie VARCHAR, nazwisko VARCHAR, wiek INT, data_ur DATE, pensja MONEY)");
		 printf("\nDodano tabele osoba(id_osoba INTEGER PRIMARY KEY, imie VARCHAR, nazwisko VARCHAR, wiek INT, data_ur DATE, pensja MONEY)\n");
		 printf("\n");
		printf("\n");
		printf("\n");printf("\n");
        break;

        case 3:
		printf("\n---------------------------------\n");
		printf("USUWANIE TABELI OSOBA");
		printf("\n---------------------------------\n");
		doSQL(conn, "DROP TABLE osoba");
		printf("\nTabela osoba zostala usunieta\n");
		printf("\n");
		printf("\n");
		printf("\n");printf("\n");
        break;

        case 4:
		printf("\n---------------------------------\n");
		printf("TWORZENIE WYZWALACZA");
		printf("\n---------------------------------\n");
	doSQL(conn, "CREATE OR REPLACE FUNCTION wiek() RETURNS TRIGGER AS $$ BEGIN IF NEW.wiek < 16 THEN RAISE EXCEPTION 'OSOBA JEST ZA MLODA'; END IF; IF NEW.pensja::NUMERIC < 0 THEN RAISE EXCEPTION 'Pensja nie moze byc mniejsza niz 0!'; END IF; RETURN NEW; END; $$ LANGUAGE 'plpgsql';");
doSQL(conn, "CREATE TRIGGER wyzwalacz BEFORE INSERT OR UPDATE ON osoba FOR EACH ROW EXECUTE PROCEDURE wiek();");
		printf("\n");
		printf("\n");
		printf("\n");printf("\n");
        break;

		case 5:
		printf("\n---------------------------------\n");
		printf("USUWANIE WYZWALACZA");
		printf("\n---------------------------------\n");
						doSQL(conn, "DROP TRIGGER wyzwalacz ON osoba");
		doSQL(conn, "DROP FUNCTION wiek()");
		printf("\n");
		printf("\n");
		printf("\n");printf("\n");
        break;
		
		case 6:
		printf("\n---------------------------------\n");
		printf("DODAWANIE REKORDU DO TABELI");
		printf("\n---------------------------------\n");
		printf("Podaj wartosci:\n");
		printf("ID: "); scanf("%d",&value);
		printf("Imie: "); scanf("%s",imie);
		printf("Nazwisko: "); scanf("%s",nazwisko);
		printf("Wiek: "); scanf("%d",&wiek);
		printf("Data urodzenia (YYYY-MM-DD): "); scanf("%s",data_ur);
		printf("Pensja: "); scanf("%d",&pensja);
		sprintf(zapytanie, "INSERT INTO osoba VALUES(%d, \'%s\', \'%s\', %d, \'%s\', %d)",value, imie, nazwisko, wiek, data_ur, pensja);
		doSQL(conn, zapytanie);
		printf("Wykonano polecenie INSERT INTO osoba VALUES(%d, \'%s\', \'%s\', %d, \'%s\', %d)\n",value, imie, nazwisko, wiek, data_ur, pensja);
		printf("\n");
		printf("\n");
		printf("\n");printf("\n");
        break;
		
		case 7:
		printf("\n---------------------------------\n");
		printf("UPDATOWANIE REKORDU W TABELI");
		printf("\n---------------------------------\n");
		
		doSQL(conn, "SELECT * FROM osoba");
		printf("Podaj id rekordu ktory ma byc uaktualniony:");
        scanf("%d",&value);
        sprintf(zapytanie, "SELECT * FROM osoba WHERE id_osoba = %d", value);
         doSQL(conn, zapytanie);
		printf("\n");
		printf("\n");
        printf("Ktore pole chcesz uaktualnic: \n1 = ID\n2 = Imie\n 3 = Nazwisko\n 4 = wiek\n 5 = data_ur\n 6 = pensja\nTwoj wybor: ");
         scanf("%d",&y);
			if(y == 1) {
				printf("Nowe id: ");
                scanf("%d", &nowe_id);
                sprintf(zapytanie, "UPDATE osoba SET id_osoba = %d WHERE id_osoba = %d",nowe_id,value);
             }
            else if(y == 2 ) {
                printf("Nowe imie: ");
                scanf("%s",imie);
                sprintf(zapytanie, "UPDATE osoba SET imie = \'%s\' WHERE id_osoba = %d",imie,value);
             }
            else if(y == 3 ) {
				printf("Nowe nazwisko: ");
                scanf("%s",nazwisko);
                sprintf(zapytanie, "UPDATE osoba SET nazwisko = \'%s\' WHERE id_osoba = %d",nazwisko,value);
            }
            else if(y == 4 ) {
                printf("Nowy wiek: ");
                scanf("%d", &wiek);
                sprintf(zapytanie, "UPDATE osoba SET wiek = %d WHERE id_osoba = %d",wiek,value);
            }
            else if(y == 5 ) {
                printf("Nowa data urodzenia (YYYY-MM-DD): ");
                scanf("%s",data_ur);
                sprintf(zapytanie, "UPDATE osoba SET nazwisko = \'%s\' WHERE id_osoba = %d",data_ur,value);
            }
            else if(y == 6 ) {
                printf("Nowa pensja: ");
                scanf("%d", &pensja);
                sprintf(zapytanie, "UPDATE osoba SET pensja = %d WHERE id_osoba = %d",pensja,value);
            }
                                
        doSQL(conn, zapytanie);
		printf("\n");
		printf("\n");
		sprintf(zapytanie, "SELECT * FROM osoba WHERE id_osoba = %d", value);
        doSQL(conn, zapytanie);
		printf("\n");
		printf("\n");
		
        break;
		
		case 8:
		printf("\n---------------------------------\n");
		printf("USUWANIE REKORDU Z TABELI");
		printf("\n---------------------------------\n");
		printf("Aktualnie w tabeli znajduje sie:\n");
		doSQL(conn, "SELECT * FROM osoba");
		printf("\nPodaj id rekordu, ktory chcesz usunac: "); scanf("%d", &value);
		sprintf(zapytanie, "DELETE FROM osoba WHERE id_osoba=%d", value);
		doSQL(conn, zapytanie);
		printf("\nOsoba o id_osoba = %d zostala usnieta\n", value);
		printf("\n");
		printf("\n");
		printf("\n");printf("\n");
        break;
		
		case 9:
		printf("\n---------------------------------\n");
		printf("WYSWIETLANIE REKORDOW Z  TABELI");
		printf("\n---------------------------------\n");
		printf("Dane z tabeli: \n");
		doSQL(conn, "SELECT * FROM osoba");
		printf("\n");
		printf("\n");
		printf("\n");printf("\n");
        break;
		
		case 10:
		printf("\n---------------------------------\n");
		printf("WYSZUKIWANIE REKORDOW W  TABELI");
		printf("\n---------------------------------\n");
				while(z!=0){
			printf("Zakonczenie pracy wyszukiwarki = 0\nWyszukiwanie osob o podanym wieku = 1\nWyszukiwanie osob o podanym imieniu = 2\nWyszukiwanie osob o pensji wiekszej niz XXXzl = 3\n");
			printf("\n");
			printf("Twoj wybor: ");
			scanf("%i",&z);
			printf("\n");
			switch(z){
			
				case 1:
				printf("Wyszukaj osoby w wieku: "); scanf("%d", &wiek);
				sprintf(zapytanie, "SELECT * FROM osoba WHERE wiek=%d", wiek);
				doSQL(conn, zapytanie);
				printf("\n");printf("\n");
				break;
				
				case 2:
				printf("Wyszukaj osobe o imieniu : "); scanf("%s", imie);
				printf("\n");
				sprintf(zapytanie, "SELECT * FROM osoba WHERE imie=\'%s\'", imie);
				doSQL(conn, zapytanie);printf("\n");printf("\n");
				break;
				
				case 3:
				printf("Wyszukaj osoby o pensji wiekszej niz: "); scanf("%d", &pensja);
				sprintf(zapytanie, "SELECT * FROM osoba WHERE pensja::NUMERIC>%d", pensja);
				doSQL(conn, zapytanie);
				printf("\n");printf("\n");
				break;
					}
					break;
		}
		break;
        }
 }
  PQfinish(conn);
  return EXIT_SUCCESS;

return 0;
}

