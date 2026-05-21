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

// Tempo máximo da execução de um processo por entrada na CPU (microsegundos)
extern int QUANTUM;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected; 
 
    if (current != NULL)
    {
        // Calcula percentual de QUANTUM utilizado pelo processo
        int percentual_quantum_usado = (current->process_time * 100) / QUANTUM;
        
        // Feedback dinâmico: classifica a fila baseado no uso do QUANTUM
        if (percentual_quantum_usado < 50)
        {
            // Processo que usa pouco QUANTUM (I/O bound) vai para Fila 1
            current->queue = 0;
            
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
        else
        {
            // Processo que usa muito QUANTUM (CPU bound) vai para Fila 2
            current->queue = 1;
            
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
    }
 
    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }
 
    struct queue * targetQueue = NULL;
    int queueCode = 0;

    // Seleção probabilística: 70% Fila 1, 30% Fila 2
    if (!isempty(ready) && (isempty(ready2) || make_rand(100) < 70))
    {    
        targetQueue = ready;
        queueCode = 0;
    }
    else
    {
        targetQueue = ready2;
        queueCode = 1;
    }
 
    selected = dequeue(targetQueue);
 
    selected->queue = queueCode;
    selected->state = RUNNING;
 
    return selected;
}