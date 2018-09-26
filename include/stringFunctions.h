/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/

int StringContains(char *, char, int); // Verifica a quantidade de vezes que um item aparece na string
int StringContainsAtEnd(char *, char, int); // Verifica se a string contem um caracter na sua ultima posição preenchida (retorna 1 se tiver, 0 se não)
void ClearString(char *, int); // Limpa a string por completo botando '\0' em todos itens, de acordo com seu tamanho
void RemoveChar(char, char *, int, int); // Remove apenas o primeiro e o ultimo elemento encontrado em 'removeChar', ou apenas o ultimo elemento se especificado pelo ultimo int com 1 (0 para inicio ou fim)
