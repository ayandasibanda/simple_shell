#include "shell.h"

/**
 * _memset - fills up memory with constant byte
 * @s: addressof memory
 * @b: the byte to fill *s with
 * @n: the number of bytes to fill s
 * Return: address to the memory
 */
char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		s[i] = b;
	return (s);
}

/**
 * ffree - frees an array of strings
 * @pp: arr of strings
 */
void ffree(char **pp)
{
	char **a = pp;

	if (!pp)
		return;
	while (*pp)
		free(*pp++);
	free(a);
}

/**
 * _realloc - will reallocate memory to pointer
 * @ptr: pointer to prev bloc of memory
 * @old_size:size of prev memory block
 * @new_size: size of new block of memory
 *
 * Return: adress of new memory
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	p = malloc(new_size);
	if (!p)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (p);
}
