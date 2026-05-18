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

    if (current != NULL)
    {
        switch (current->state) 
        {
            case READY:
                enqueue(ready, current);
                break;
            case BLOCKED:
                enqueue(blocked, current);
                break;
            case FINISHED:
                enqueue(finished, current);
                break;
            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    if (isempty(ready))
        return NULL;

    struct proc * processoApto = ready->head,
        * processoMaisLento = NULL;

    while (processoApto != NULL)
    {
        if (processoMaisLento == NULL || processoApto->remaining_time > processoMaisLento->remaining_time)
        {
            processoMaisLento = processoApto;
        }
        processoApto = processoApto->next;
    }

    selected = processoMaisLento;
    dequeue_bypid(ready, selected->pid);

    selected->state = RUNNING;

    return selected;
}

