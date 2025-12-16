#include "get_next_line.h"

static char	*read_line(int fd, char *stash);
static char *cut_line(char *stash);
static  char    *save_remainder(char *stash);
//glowna funkcja zwracajaca kolejne linie z pliku


char	*get_next_line(int fd)
{
	static char	*stash;//stash jest static by pamietac reszte miedzy wywolaniami
	char	*line;

	if (fd < 0 || BUFFER_SIZE <= 0)//jesli fd jest error(w naszym przypadku np.3 u nas BUFFER_SIZE define 42
		return NULL;//ma zwrocic null

	stash = read_line(fd, stash);//czytamy dane z pliku i doklejamy do stash
	if (!stash)//jesli brak danych lub blad
		return (NULL);//zwroc null
	line = cut_line(stash);//wyciagmy pierwsza linie do zwrocenia
	stash = save_remainder(stash);//aktualizujemy stash, zostawiajac reszte danych na kolejne wywolanie
	return (line);//zwracamy linie uzytkownikowi
}


//read_line:
//	Czyta dane z pliku po kawalku do momentu napotkania '\n' lub konca pliku.//jesli buffor np 42 to czyta caly plik
//	stash: poprzednie dane z poprzedniego wywolania get_next_line.
//	read_line - zwraca nowy, polaczony string (stash + nowe dane) az do '\n'
//
//	przyklad dla pliku;
//	"leo\nleonardo\0"

static char	*read_line(int fd, char *stash)
{
	char	*buffer;	//tymczasowy buffer do odczytu
	ssize_t	bytes_read;	// ile bajtow odczytalismy z pliku test.txt
	
	buffer = malloc(BUFFER_SIZE + 1);//alokujemy bufor 42 charow + 1 char (na'\0')
	if (!buffer)	//jesli alokacja sie nie powiodla
		return (NULL);	//zwracamy null
	bytes_read = 1; //inicjemy zmienna by wejsc do petli
	//petla czyta, dopoki w stashu nie znajdzie '\n' i read() nie zwroci 0
	while (!ft_strchr(stash, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE); // czytamy maks 42 bajtow z fd
		if (bytes_read < 0) //jesli blad odczytu
		{
			free(buffer);	//zwalniamy buffor
			return (NULL);	//konczymy funkcje
		}
		buffer[bytes_read] = '\0';  //konczymy string w buforze
		/*
		przyklad
		plik: "leo\nleonardo\0"
		BUFFER_SIZE = 42
	1:
		pierwsze wywolanie read(linia 50)
		bytes_read = 13
		buffer = "leo\nLeonardo\0"
	2:
		stash = NULL (pierwsze wywolanie get_next_line
		stash = ft_strjoin(NULL, buffer)
		stash = "leo\nLeonardo\0"
	3:
		Sprawdzanie ft_strchr(stash, '\n')
		'\n' jest w stash -> petla while sie konczy

		-> stash teraz zawiera wszystkie dane z pliku do pierwszego \n
		-> buffer zostaje zwolniony
		*/
	stash = ft_strjoin(stash, buffer); //laczymy stary stash z bufforem
	if (!stash)
	{
		free(buffer); //zwalniamy buffor
		return (NULL);	//konczymy funkcje
	}
	
}
	free(buffer);	//zwalniamy buffor bo nie jest juz potrzebny

	// stan po wyjsciu z petli:
	// stash = "leo\nLeonardo\0"
	// - zawiera pierwsza linie i reszte pliku
	// gotowe do uzycia w cut_line() i save_remainder()


	
	return (stash);
}

//	cut_line
//	wycina pierwsza linie ze stash
//	zwraca nowy string (linie do zwrocenia)
static char *cut_line(char *stash)
{
	size_t	i; //indeks do iteracji po stash

	if (!stash || stash[0] == '\0') //jesli stash jest NULL lub pusty
		return (NULL);	//zwroc NULL

	i = 0;//  zaczynamy od poczatku stringa - u nas stash = "leo\nLeonardo\0"
	while(stash[i] && stash[i] != '\n')// idziemy do konca stringa lub znaku '\n'
		i++;
	if (stash[i] == '\n') //jesli napotkalismy '\n'
		// przyklad
		// stash = "leo\n"Leonardo\0"
		// i = 3 bo stash[3] = '\n')
		// ft_substr(stash, 0, 4) tworzy i zwraca -> "leo\n"
		return (ft_substr(stash, 0, i + 1));
	
	// jesli np. nasz stash = "leonardo\0"
	// i = 8
	// zwraca i tworzy ft_substr(stash, 0, 8) -> leonardo
	return (ft_substr(stash, 0, i));
}

//	save_remainder
//	zachowuje w stash wszystko po pierwszej lini	
//	zwraca nowy stash
//
static	char	*save_remainder(char *stash)
{
	size_t	i; //indeks do szukania pierwszej lini
	char	*remain;// nowy string z reszta danych

	i = 0;
	while (stash[i] && stash[i] != '\n')// szukamy znaku '\n' w stash
		i++;// iteracja
	if (!stash[i])// jesli nie ma '\n' to nie ma reszty do zapisania
	{
		free(stash); // zwalniamy stash
		return (NULL); // brak reszty
	}

	// tworzymy nowy string z reszty po '\n'
	// ft_substr(stash, i + 1, ft_strlen(stash) - i - 1)
	// i + 1 -> zaczynamy zaraz po '\n'
	// ft_strlen(stash) - i - 1 -> ile znakow zostalo do konca
	remain = ft_substr(stash, i + 1, ft_strlen(stash) - i -1);

	free(stash); // zwalniamy stary stash
	return (remain); // nowy stash do nastepnego wywolania
	/*np.
	stash = "leo\nLeonardo\0"
	i = 3 (stash[3] = '\n')
	ft_substr(stash, 4, 13 - 3 - 1 = 9) -> "leonardo\0"
	nowy stash = "leonardo\0" */
}