#include <stdio.h>

#include "queue.h" // contem funções uteis para filas
#include "proc.h"  // possui as funções dos processos
#include "stats.h" // possui as funções de estatisticas 
#include "utils.h" // possui funções uteis 

// Utilizando as variáveis globais definidas no 'main'
extern struct queue * ready;    // fila de aptos
extern struct queue * ready2;   // segunda fila de aptos
extern struct queue * blocked;  // fila de bloqueados
extern struct queue * finished; // fila de finalizados
// NOTE: essa fila de finalizados é utilizada apenas para
// as estatisticas finais

// variavel global que indica o tempo maximo que um processo pode executar ao todo
extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected;

    /*
     * Tratando o processo que está atualmente executando
     */
    
    if (current != NULL)
    {
        switch (current->state) 
        {
            // Caso esteja 'apto' é porque saiu por preempção
            // Preempção → Retorna para a Fila 1 (ready)
            case READY:
                current->queue = 0;  // Marcar como pertencente à Fila 1
                enqueue(ready, current);
                break;

            // Caso tenha saído como 'bloqueado' é porque fez uma E/S
            // E/S → Retorna para a Fila 2 (ready2)
            case BLOCKED:
                current->queue = 1;  // Marcar como pertencente à Fila 2
                enqueue(blocked, current);
                break;

            // Caso o processo tenha finalizado
            case FINISHED:
                enqueue(finished, current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    /*
     * Estratégia de seleção com realimentação dinâmica
     */

    // Se ambas as filas estão vazias, não há o que fazer
    if (isempty(ready) && isempty(ready2))
        return NULL;

    // Seleção probabilística entre as filas
    struct queue * targetQueue = NULL;
    int queueCode = 0;

    // Gera um número de 0 a 99
    // Se < 70, seleciona Fila 1; caso contrário, seleciona Fila 2
    if (!isempty(ready) && (isempty(ready2) || make_rand(100) < 70))
    {    
        // Seleciona da Fila 1 (ready)
        targetQueue = ready;
        queueCode = 0;
    }
    else
    {
        // Seleciona da Fila 2 (ready2)
        targetQueue = ready2;
        queueCode = 1;
    }

    // Remove o primeiro processo da fila selecionada (FIFO)
    selected = dequeue(targetQueue);

    // Marca qual fila o processo pertence
    selected->queue = queueCode;

    // Altera o estado para executando
    selected->state = RUNNING;

    return selected;
}