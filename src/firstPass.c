/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/

#ifndef _Primary_libraries
  #define _Primary_libraries
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
#endif

#ifndef _PreProcess_library
  #define _PreProcess_library
    #include "preProcess.h"
#endif

#ifndef _StringFunctions_library
  #define _StringFunctions_library
    #include "stringFunctions.h"
#endif

#ifndef _FirstPass_library
  #define _FirstPass_library
    #include "firstPass.h"
#endif

// isModule - 1: tem Begin, 0 - : não tem Begin. Marca o inicio e fim do 'text' do modulo
objCode * DoFirstPass(preProcess *preProcessHead, symbolTable **symbolTableHead, definitionTable **definitionTableHead, int *isModule)
{
  objCode *objCodeHead = NULL;
  char item[51], Operator1[51], Operator2[51];
  // Section = -1: indefinido,  1 - TEXT, 2 - DATA, 3 - BSS
  int locationCounter = 0, section = -1, isEndOfLine = 0, directiveValue = 0, argummentsN = 0, needEndOfLine = 0, wasEnd = 0, wasText = 0, wasLabel = 0;
  int Opcode = -1, OpcodeLocationCouter = -1, Operator1LocationCouter = -1, isRelative1 = -1, Operator2LocationCouter = -1, isRelative2 = -1;

  // Limpa os dados do Operator1 e Operator2
  ClearString(Operator1, 51);
  ClearString(Operator2, 51);

  if(preProcessHead != NULL)
  {
    while(preProcessHead != NULL)
    {
      // needEndOfLine é 1 quando foi executado tudo que deveria ser na linha, usado para identificar se ainda
      // tem algo nela, e para gerar um erro.
      if(needEndOfLine)
        printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);

      // Pega o primeiro item da lista do programa pre-processado
      GetItem(preProcessHead, item, &isEndOfLine);

      // Verifica se é uma label comum do programa (label:) - nunca será externo nesse caso
      if(isLabelDeclaration(item))
      {
        AddSymBolTable(symbolTableHead, item, locationCounter, 0, preProcessHead->LineCounter);
        wasLabel = 1;
      }

      // Executa a diretiva encontrada no ultimo item
      if(directiveValue != 0)
      {
        switch (directiveValue)
        {
          case 1: // Section
            if(isWhichSection(item, &section, wasEnd, preProcessHead->LineCounter) == 0)
              printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);

            argummentsN = 0;
            needEndOfLine = 1;

            if(section == 1)
              wasText = 1;

            break;

          case 2: // Space

            strcat(Operator1, item);
            argummentsN -= 1; // Foi add um argumento

            if(item[0] == '+' && item[1] != '\0' || StringContainsAtEnd(item, '+', 51))
              argummentsN += 1; // Caso tenha uma soma junto a algum operando, tem que somar 1 (pois serão só 2 itens)

            if(item[0] == '+' && item[1] == '\0')
              argummentsN += 2; // Caso tenha uma soma sozinha, tem que adicionar 2 (pois tera um argumento na esquerda e um na direita)

            if(isEndOfLine)
            {
              Operator1LocationCouter = locationCounter -1;
              isRelative1 = 1;
            }

            break;

          case 3: // Const
            if(item[0] >= 0x30 && item[0] <= 0x39 || item[0] == 0x2D && item[1] >= 0x30 && item[1] <= 0x39 || item[0] == 0x2D && item[1] == '\0')
            {
              needEndOfLine = 1;
              argummentsN = 0;
              Operator1LocationCouter = locationCounter -1;
              strcat(Operator1, item);
              isRelative1 = 0;
              // Sera que pode labels ou somas no const? não está sendo considerado!! (apenas numeros positivo ou negativos)
            }
            else
            {
              printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);
            }

            break;

          case 4: // Public
            AddDefinitionTableLabel(definitionTableHead, item, preProcessHead->LineCounter);
            needEndOfLine = 1;
            break;

          case 5: // Extern
            AddSymBolTable(symbolTableHead, item, 0, 1, preProcessHead->LineCounter);
            needEndOfLine = 1;
            break;

          case 6: // Begin
            printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);
            break;

          case 7: // End
            printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);
            break;
        }
      }

      // Verifica se é uma diretiva (não recheca, pois pode acontecer duas etapas (primeira para pegar o sinal do numero, segunda para pegar o numero))
      if(directiveValue == 0)
        directiveValue = isDirective(item, &argummentsN, &locationCounter, &section, preProcessHead->LineCounter, isModule);

      // Se houve um Space (não necessariamente precisa de um operador, quando não possui operador, assume-se 1)
      if(directiveValue == 2 && isEndOfLine && Operator1[0] == '\0')
      {
          Operator1LocationCouter = locationCounter -1;
          isRelative1 = 1;
          Operator1[0] = 0x31;
          argummentsN = 0;
      }

      // Seta que houve um Begin
      if(directiveValue == 6)
        wasEnd = 1;

      // Seta que houve um End
      if(directiveValue == 7)
        wasEnd = 0;

      // Executa o opcode encontrado no ultimo item
      if(Opcode != -1)
      {
        switch (Opcode)
        {
          case 9: // COPY
            argummentsN -= 1; // Foi add um argumento

            if(item[0] == ',' && item[1] == '\0') // Caso a virgula venha só
            {
              argummentsN += 1; // Desconsidera a passagem dela
            }

            if(Operator1[0] != '\0')
            {
              // Adiciona os itens no Operator2
              strcat(Operator2, item);

              if(item[0] == '+' && item[1] != '\0' || StringContainsAtEnd(item, '+', 51))
                argummentsN += 1; // Caso tenha uma soma junto a algum operando, tem que somar 1 (pois serão só 2 itens)

              if(item[0] == '+' && item[1] == '\0')
                argummentsN += 2; // Caso tenha uma soma sozinha, tem que adicionar 2 (pois tera um argumento na esquerda e um na direita)

              if(isEndOfLine) // Adiciona os itens do Operator2
              {
                Operator1LocationCouter = locationCounter;
                isRelative1 = 1;
                locationCounter += 1;
                Operator2LocationCouter = locationCounter;
                locationCounter += 1;
                isRelative2 = 1;
              }
            }
            else
            {
              strcpy(Operator1, item);
            }

            break;

          case 14: // STOP
            printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);
            break;

          default: // ADD SUB MULT DIV JMP JMPN JMPP JMPZ LOAD STORE INPUT OUTPUT

            // Adiciona os itens no Operator1
            strcat(Operator1, item);
            argummentsN -= 1; // Foi add um argumento

            if(item[0] == '+' && item[1] != '\0' || StringContainsAtEnd(item, '+', 51))
              argummentsN += 1; // Caso tenha uma soma junto a algum operando, tem que somar 1 (pois serão só 2 itens)

            if(item[0] == '+' && item[1] == '\0')
              argummentsN += 2; // Caso tenha uma soma sozinha, tem que adicionar 2 (pois tera um argumento na esquerda e um na direita)

            if(isEndOfLine)
            {
              Operator1LocationCouter = locationCounter;
              locationCounter += 1;
              isRelative1 = 1;
            }

            break;
        }
      }

      // Verifica se é um Opcode se não tiver diretivas. Não rechecar se já reconheceu um opcode.
      if(Opcode == -1 && directiveValue == 0)
      {
          OpcodeLocationCouter = locationCounter;
          isOpcode(item, &argummentsN, &Opcode, &locationCounter, section, preProcessHead->LineCounter);

          // Reseta o OpcodeLocationCouter caso não tenha ocorrido algum opcode
          if(Opcode == -1)
            OpcodeLocationCouter = -1;
      }

      if(Opcode == -1 && directiveValue == 0 && isEndOfLine == 1 && wasLabel == 0)
        printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);

      // Remove o wasLabel
      wasLabel = 0;

      if(isEndOfLine)
      {
        // Caso não tenha sido SECTION TEXT no inicio do código
        if(wasText != 1)
          printf("Erro semântico na linha: %d.\n", preProcessHead->LineCounter);

        // Caso alguma diretiva / instrução execute mais do que deveria dos seus argumentos e verificação do Operator1
        if(argummentsN != 0 || Operator1[0] == '+' || StringContainsAtEnd(Operator1, '+', 51))
            printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);

        // Casos de erro com a virgula
        if(Opcode == 9 && StringContains(Operator1, ',', 51) >= 1 && StringContains(Operator2, ',', 51) >= 1
          || Opcode != 9 && StringContains(Operator1, ',', 51) >= 1 || Opcode == 9 && Operator1[0] == ','
          || Opcode == 9 && StringContains(Operator1, ',', 51) == 0 && StringContains(Operator2, ',', 51) == 0)
          printf("Erro sintático na linha: %d.\n", preProcessHead->LineCounter);

        // Remove as virgulas do Opt1 e Opt2
        RemoveChar(',', Operator1, 51, 0);
        RemoveChar(',', Operator2, 51, 0);

        // Insere dados na lista do objCode
        if(Opcode != -1 || directiveValue == 2 || directiveValue == 3)
          AddObjCode(&objCodeHead, Opcode, OpcodeLocationCouter, Operator1, Operator1LocationCouter, isRelative1, Operator2, Operator2LocationCouter, isRelative2, preProcessHead->LineCounter);

        preProcessHead = preProcessHead->nextLine;

        // Limpa todos os dados
        ClearString(Operator1, 51);
        ClearString(Operator2, 51);
        isEndOfLine = 0;
        needEndOfLine = 0;
        argummentsN = 0;
        directiveValue = 0;
        Opcode = -1;
        OpcodeLocationCouter = -1;
        Operator1LocationCouter = -1;
        Operator2LocationCouter = -1;
        isRelative1 = -1;
        isRelative2 = -1;
      }
    }
    // Copia os valores dos labels da tabela de simbolos para a tabela de definições (os definidos na tabela de definição sem valor)
    AddDefinitionTableValue((*definitionTableHead), (*symbolTableHead));
  }
  else
  {
    printf("Erro: Programa vazio.\n");
  }

  return objCodeHead;
}

void AddObjCode(objCode **objCodeHead, int Opcode, int OpcodeLocationCouter, char *Operator1, int Operator1LocationCouter, int isRelative1, char *Operator2, int Operator2LocationCouter, int isRelative2, int LineCounter)
{
  objCode *objCodeCreator, *lastElem;

  // Alocação da memória para o objCode
  objCodeCreator = (objCode *) malloc(sizeof(objCode));

  if(*objCodeHead == NULL)
  {
    (*objCodeHead) = objCodeCreator;
    (*objCodeHead)->nextLine = NULL;
    (*objCodeHead)->previousLine = NULL;
  }
  else
  {
    lastElem = *objCodeHead;

    while(lastElem->nextLine != NULL)
      lastElem = lastElem->nextLine;

    objCodeCreator->previousLine = lastElem;
    lastElem->nextLine = objCodeCreator;
    objCodeCreator->nextLine = NULL;
  }

  // Copia os dados para o novo item do objCode
  objCodeCreator->Opcode = Opcode;
  objCodeCreator->OpcodeLocationCouter = OpcodeLocationCouter;
  strcpy(objCodeCreator->Operator1, Operator1);
  objCodeCreator->Operator1LocationCouter = Operator1LocationCouter;
  objCodeCreator->isRelative1 = isRelative1;
  strcpy(objCodeCreator->Operator2, Operator2);
  objCodeCreator->Operator2LocationCouter = Operator2LocationCouter;
  objCodeCreator->isRelative2 = isRelative2;
  objCodeCreator->LineCounter = LineCounter;
}

void AddSymBolTable(symbolTable **symbolTableHead, char *Label, int Value, int isExtern, int LineCounter)
{
  symbolTable *symbolTableCreator, *lastElem;

  // Remoção do ':' do label.
  RemoveChar(':', Label, 51, 1);

  if(SymbolTableContains(*symbolTableHead, Label) == 0 && SymbolTableContainsValue(*symbolTableHead, Value) == 0)
  {
    // Alocação da memória para o symbolTable
    symbolTableCreator = (symbolTable *) malloc(sizeof(symbolTable));

    if(*symbolTableHead == NULL)
    {
      (*symbolTableHead) = symbolTableCreator;
      (*symbolTableHead)->nextItem = NULL;
      (*symbolTableHead)->previousItem = NULL;
    }
    else
    {
      lastElem = *symbolTableHead;

      while(lastElem->nextItem != NULL)
        lastElem = lastElem->nextItem;

      symbolTableCreator->previousItem = lastElem;
      lastElem->nextItem = symbolTableCreator;
      symbolTableCreator->nextItem = NULL;
    }

    // Copia os dados para o novo item do symbolTable
    strcpy(symbolTableCreator->Label, Label);
    symbolTableCreator->Value = Value;
    symbolTableCreator->isExtern = isExtern;
  }
  else
  {
    printf("Erro semântico na linha: %d.\n", LineCounter);
  }
}

// Checa se a tabela de simbolos ja contem o label, retornando 1 se contem, 0 se não.
int SymbolTableContains(symbolTable *symbolTableHead, char *Label)
{
  if(symbolTableHead != NULL)
  {
    while(symbolTableHead != NULL)
    {
      if(strcmp(symbolTableHead->Label, Label) == 0)
        return 1;

      symbolTableHead = symbolTableHead->nextItem;
    }
  }
  else
  {
    return 0;
  }
}

// Checa se está sendo add mais de um label em uma unica linha
int SymbolTableContainsValue(symbolTable *symbolTableHead, int Value)
{
  if(symbolTableHead != NULL)
  {
    while(symbolTableHead != NULL)
    {
      if(symbolTableHead->Value == Value)
        return 1;

      symbolTableHead = symbolTableHead->nextItem;
    }
  }
  else
  {
    return 0;
  }
}

// Adiciona apenas o label na tabela de definições, deixando o valor preenchido com a linha do label (para identificação do erro quando não se encontrar o simbolo na tabela de simbolos)
void AddDefinitionTableLabel(definitionTable **definitionTableHead, char *Label, int lineCounter)
{
  definitionTable *definitionTableCreator, *lastElem;

  // Alocação da memória para o definitionTable
  definitionTableCreator = (definitionTable *) malloc(sizeof(definitionTable));

  if(*definitionTableHead == NULL)
  {
    (*definitionTableHead) = definitionTableCreator;
    (*definitionTableHead)->nextItem = NULL;
    (*definitionTableHead)->previousItem = NULL;
  }
  else
  {
    lastElem = *definitionTableHead;

    while(lastElem->nextItem != NULL)
      lastElem = lastElem->nextItem;

    definitionTableCreator->previousItem = lastElem;
    lastElem->nextItem = definitionTableCreator;
    definitionTableCreator->nextItem = NULL;
  }

  // Copia os dados para o novo item do definitionTable
  strcpy(definitionTableCreator->Label, Label);
  definitionTableCreator->Value = lineCounter;
}

// Adiciona o valor dos itens definidos na tabela de simbolos na tabela de definições que ainda estão sem valor.
void AddDefinitionTableValue(definitionTable *definitionTableHead, symbolTable *symbolTableHead)
{
  symbolTable *auxSymbolTable;

  if(definitionTableHead != NULL && symbolTableHead != NULL)
  {
    while(definitionTableHead != NULL)
    {
      auxSymbolTable = symbolTableHead;
      while(auxSymbolTable != NULL)
      {
        if(strcmp(auxSymbolTable->Label, definitionTableHead->Label) == 0)
        {
          definitionTableHead->Value = auxSymbolTable->Value;
          break;
        }

        if(auxSymbolTable->nextItem != NULL)
        {
          auxSymbolTable = auxSymbolTable->nextItem;
        }
        else
        {
          break;
        }
      }
      // Caso o elemento não tenha sido achado na tabela de simbolos
      if(strcmp(auxSymbolTable->Label, definitionTableHead->Label) != 0)
      {
        printf("Erro semântico na linha: %d.\n", definitionTableHead->Value);
        definitionTableHead->Value = -1; // Bota valor equivalente ao 'null'
      }

      definitionTableHead = definitionTableHead->nextItem;
    }
  }
}

void DeleteObjCode(objCode **objCodeHead)
{
  objCode *aux;

  while(*objCodeHead != NULL)
  {
    aux = *objCodeHead;
    *objCodeHead = (*objCodeHead)->nextLine;
    free(aux);
  }
}

void DeleteSymbolTabel(symbolTable **symbolTableHead)
{
  symbolTable *aux;

  while(*symbolTableHead != NULL)
  {
    aux = *symbolTableHead;
    *symbolTableHead = (*symbolTableHead)->nextItem;
    free(aux);
  }
}

void DeleteDefinitionTable(definitionTable **definitionTableHead)
{
  definitionTable *aux;

  while(*definitionTableHead != NULL)
  {
    aux = *definitionTableHead;
    *definitionTableHead = (*definitionTableHead)->nextItem;
    free(aux);
  }
}

// No 'argummentsN' a quantidade de argumentos do opcode, o opcode e o tamanho da operação em memoria
void isOpcode(char *operation, int *argummentsN, int *Opcode, int *size, int section, int lineCounter)
{
  if(strcmp(operation, "ADD") == 0)
  {
    *argummentsN = 1;
    *Opcode = 1;
    *size += 1;
  }
  else if(strcmp(operation, "SUB") == 0)
  {
    *argummentsN = 1;
    *Opcode = 2;
    *size += 1;
  }
  else if(strcmp(operation, "MULT") == 0)
  {
    *argummentsN = 1;
    *Opcode = 3;
    *size += 1;
  }
  else if(strcmp(operation, "DIV") == 0)
  {
    *argummentsN = 1;
    *Opcode = 4;
    *size += 1;
  }
  else if(strcmp(operation, "JMP") == 0)
  {
    *argummentsN = 1;
    *Opcode = 5;
    *size += 1;
  }
  else if(strcmp(operation, "JMPN") == 0)
  {
    *argummentsN = 1;
    *Opcode = 6;
    *size += 1;
  }
  else if(strcmp(operation, "JMPP") == 0)
  {
    *argummentsN = 1;
    *Opcode = 7;
    *size += 1;
  }
  else if(strcmp(operation, "JMPZ") == 0)
  {
    *argummentsN = 1;
    *Opcode = 8;
    *size += 1;
  }
  else if(strcmp(operation, "COPY") == 0)
  {
    *argummentsN = 2;
    *Opcode = 9;
    *size += 1;
  }
  else if(strcmp(operation, "LOAD") == 0)
  {
    *argummentsN = 1;
    *Opcode = 10;
    *size += 1;
  }
  else if(strcmp(operation, "STORE") == 0)
  {
    *argummentsN = 1;
    *Opcode = 11;
    *size += 1;
  }
  else if(strcmp(operation, "INPUT") == 0)
  {
    *argummentsN = 1;
    *Opcode = 12;
    *size += 1;
  }
  else if(strcmp(operation, "OUTPUT") == 0)
  {
    *argummentsN = 1;
    *Opcode = 13;
    *size += 1;
  }
  else if(strcmp(operation, "STOP") == 0)
  {
    *argummentsN = 0;
    *Opcode = 14;
    *size += 1;
  }

  if(section != 1 && *Opcode != -1)
    printf("Erro semântico na linha: %d.\n", lineCounter);
}

// retorna 0 se não for diretiva, numero da diretiva se for. primeiro item é o numero de operandos e o segundo item o tamanho.
int isDirective(char *directive, int *argummentsN, int *size, int *section, int lineCounter, int *isModule)
{
  if(strcmp(directive, "SECTION") == 0)
  {
    *argummentsN = 1;
    *size += 0;
    return 1;
  }
  else if(strcmp(directive, "SPACE") == 0)
  {
    *argummentsN = 1;
    *size += 1;

    if(*section != 3)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 2;
  }
  else if(strcmp(directive, "CONST") == 0)
  {
    *argummentsN = 1;
    *size += 1;

    if(*section != 2)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 3;
  }
  else if(strcmp(directive, "PUBLIC") == 0)
  {
    *argummentsN = 0;
    *size += 0;

    if(*section != 1 || *isModule != 1)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 4;
  }
  else if(strcmp(directive, "EXTERN") == 0)
  {
    *argummentsN = 0;
    *size += 0;

    if(*section != 1 || *isModule != 1)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 5;
  }
  else if(strcmp(directive, "BEGIN") == 0)
  {
    *argummentsN = 0;
    *size += 0;
    *isModule = 1;

    if(*section != 1)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 6;
  }
  else if(strcmp(directive, "END") == 0)
  {
    *argummentsN = 0;
    *size += 0;

    if(*section != 1 || *isModule != 1)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 7;
  }
  else
  {
    return 0;
  }
}

// Retorna 1 se for uma declaração de label, 0 se não
int isLabelDeclaration(char *Label)
{
  return StringContainsAtEnd(Label, ':', 51);
}

// Verifica se é TEXT, DATA ou BSS, retornando 1 se é algo, 0 se nenhum (altera o int de acordo com o formato section)
int isWhichSection(char *item, int *section, int wasEnd, int lineCounter)
{
  if(strcmp(item, "TEXT") == 0)
  {
    *section = 1;
    return 1;
  }
  else if(strcmp(item, "DATA") == 0)
  {
    *section = 2;

    if(wasEnd != 0)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else if(strcmp(item, "BSS") == 0)
  {
    *section = 3;

    if(wasEnd != 0)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else
  {
    return 0;
  }
}

// Retorna o item do programa, e verifica se é o fim da linha ou não
void GetItem(preProcess *preProcessLine, char *item, int *isEndOfLine)
{
  int i, j;
  char vector[204];

  ClearString(item, 51);
  ClearString(vector, 204);

  for(i = 0; i < 51; i++)
  {
    if(preProcessLine->Program[i] != 0x20 && preProcessLine->Program[i] != '\0')
    {
      item[i] = preProcessLine->Program[i];
    }
    else
    {
      if(preProcessLine->Program[i] == '\0')
        *isEndOfLine = 1;

      break;
    }
  }

  i += 1;
  for(j = 0; i < 204; j++, i++)
    vector[j] = preProcessLine->Program[i];

  ClearString(preProcessLine->Program, 204);
  strcpy(preProcessLine->Program, vector);
}
