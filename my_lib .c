/*
 * DIEGO BERMEJO CABAÑAS
 * EDUARDO BONNÍN NARVÁEZ
 * PERE JOAN VIVES MOREY
*/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "my_lib.h"

//RETO 1
//metodo que devuelve la longitud de un string con el caracter nulo
//parametro el string
size_t my_strlen(const char *str)
{
    size_t i = 0;
    //bucle hasta caracter nulo
    while (*(str + i))
    {
        i++;
    }
    //devuleve la longitud
    return i;
}
//metodo que compara dos strings de caracteres
int my_strcmp(const char *str1, const char *str2)
{
    int i = 0;
    //bucle mientras sean iguales i no llegue al final
    while ((str1[i] == str2[i]) && (i != my_strlen(str1)))
    {
        i++;
    }
    //devuelve la diferencia en codigo ascii de los dos caracteres
    return str1[i] - str2[i];
}
//metodo que copia un string en otra cadena
char *my_strcpy(char *dest, const char *src)
{
    int i = 0;
    //bucle recorrido 
    while (i < my_strlen(dest) || i < my_strlen(src))
    {
        //se asignan los caracteres fuente en el destino
        dest[i] = src[i];
        i++;
    }
    //devuelve la cadena
    return dest;
}
//copia n caracteres de string en otra cadena 
char *my_strncpy(char *dest, const char *src, size_t n)
{
    int i = 0;
    // memset(dest, 0, sizeof(dest)); // inicialización de *dest con el carácter nulo
    while (i < my_strlen(dest) || i < my_strlen(src))
    {   
        //si es menor se copia
        if (i < n)
        {
            dest[i] = src[i];
        }
        i++;
    }
    //devuelve la cadena
    return dest;
}
//metodo que añade la cadena apuntada por src a la apuntada por dest
char *my_strcat(char *dest, const char *src)
{
    //variables que iteran
    int i = 0, j = 0;
    //longitud de dest
    while (dest[i])
    {
        i++;
    }
    //asiganmos los char de src a la parte final de dest
    while (j < my_strlen(src))
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    //devuelve cadena apuntada por dest
    return dest;
}
//metodo que busca la primera ocurrencia del carácter c en *s
char *my_strchr(const char *s, int c)
{
    int i = 0;
    //bucle que busca el caracter c
    while (s[i] != c)
    {
        i++;
    }
    //devuelve puntero a c
    if (i < my_strlen(s))
    {
        return (char *)s + i;
    }
    //devuelve NULL si no lo encuentra
    else
    {
        return NULL;
    }
}

//RETO 2

//Reserva espacio para una variable de tipo struct my_stack
struct my_stack *my_stack_init(int size)
{
    struct my_stack *stack;
    stack = malloc(sizeof(struct my_stack));//reserva espacio
    stack->size = size;//parametro se asigna al size de la estructura
    stack->top = NULL;//top de la pila a null
    return stack;//devolvemos pila
}
//método que añade un nuevo nodo a la pila a partir de sus datos
int my_stack_push(struct my_stack *stack, void *data)
{
    
    struct my_stack_node *nodo;
    //reseramos espacio para el nodo
    nodo = malloc(sizeof(struct my_stack_node));
    //le añadimos los datos pasados por parámetro
    nodo->data = data;

    //si la pila no existe devolvemos un -1
    if (stack == NULL || stack->size < 0)
    {
        return -1;
    }
    //si no hay nodos en la pila
    if (stack->top == NULL)
    {
        //añadimos un nodo 
        stack->top = nodo;
        nodo->next = NULL;
        return 0;
    }
    else
    //si ya había nodos en la pila
    {
        //enlazamos el nodo nuevo con el anterior
        nodo->next = stack->top;
        stack->top = nodo;
        return 0;
    }
}

int my_stack_len(struct my_stack *stack)
{
    int n = 0;
    if (stack->top == NULL)
    {
        return 0; //si no hay nodos en la pila
    }
    else
    {   //apuntamos al nodo superior 
        struct my_stack_node *aux = stack->top;
        while (aux != NULL)
        {
            //hasta que no hayamos recorrido todos los nodos
            aux = aux->next;
            //vamos contando los nodos
            n++;
        }
        //devolvemos el numero de nodos de la pila
        return n;
    }
}
//método que por un lado, elimina el nodo superior de una pila
//i por otro lado, devuelve los datos que tenia ese nodo
void *my_stack_pop(struct my_stack *stack)
{
    if (stack->top != NULL)//si la pila no está vacía
    {
        void *info;
        info = stack->top->data;//cogemos los datos del nodo al que apunta top
        struct my_stack_node *n = stack->top; //apuntamos al nodo superior
        stack->top = stack->top->next;//hacemos que el puntero top apunte al nuevo nodo superior
        free(n); //liberamos el nodo superior anterior
        return info; //devolvemos los datos
    }
    else
    {
        return NULL;
    }
}
//metodo que recorre la pila liberando la memoria que se habia reservado
//devuelve el numero de bytes liberados
int my_stack_purge(struct my_stack *stack)
{
    //Bytes de la pila
    int n = sizeof(struct my_stack);
     //Recorrido por toda la pila
    while (stack->top)
    {
        n = n + sizeof(struct my_stack);//Bytes de cada nodo
        n = n + stack->size;//Bytes de cada nodo
        free(my_stack_pop(stack));//my_stack_pop retorna la posición de los datos
                                  //que eliminamos de la pila
    }
    //liberamos toda la pila
    free(stack);
    return n;
}

//método que escribe los datos de los nodos de una pila en un fichero
//devuelve el número de datos que hemos escrito
int my_stack_write(struct my_stack *stack, char *filename){
    //inicializamos una pila auxiliar
    struct my_stack *copia = my_stack_init(64); //el size serà el tamaño del struct my_data : 64
    struct my_stack_node *aux = stack->top; //vamos a utilizar un nodo auxiliar
    void *info;
    int num,contador =0,fd;
    int n = copia->size; //metemos el tamaño en n
    info = &n; //i luego a la variable info

    //metemos los datos de la pila stack en la pila copia
    for(int i =0;i<my_stack_len(stack);i++){
      my_stack_push(copia,aux->data);
      aux = aux->next;
    }
    //abrimos el fichero
    fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR);
    //metemos el tamaño que tendrán los datos como entero en el fichero
    num = write(fd,info,sizeof(int));

    //vamos metiendo los datos de cada nodo, de la pila copia, en el fichero
    for(int j =0;j<my_stack_len(stack);j++){
        info =my_stack_pop(copia);
        num =write(fd,info,n);
        contador++; //vamos contando
        
    }
    //al terminar toda operación cerramos el fichero
    close(fd);


    //si num devuelve un -1 entonces se habrá producido un error
    if(num<0){
        return -1;
    }else{
        return contador;
    }
}
//método que lee los datos de un fichero
struct my_stack *my_stack_read (char *filename){
    int fd, leidos,dato;
    void *info;
    //abrimos el fichero
    fd = open(filename,O_RDONLY);
    //leemos el primer entero, nos indica el tamaño que tendrán los datos
    leidos=read(fd,&dato,sizeof(int));
    
    //inicializamos la pila con ese tamaño
    struct my_stack *pila = my_stack_init(dato);
    //reservamos espacio
    info = malloc(dato);
    //leemos el primer dato
    leidos = read(fd,info,dato);

    //hasta que no lleguemos al final del fichero, vamos leyendo datos
    //y los introducimos en una pila
    while(leidos>0){
        my_stack_push(pila,info);
        info = malloc(dato);
        leidos = read(fd,info,dato);
    }
    //devolvemos la pila
    close(fd);
    return pila;
}

