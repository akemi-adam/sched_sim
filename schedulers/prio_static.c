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

    const int tempoLimite = MAX_TIME * 0.3;

    if (current != NULL)
    {
        switch (current->state) 
        {
            case READY:
                // Estou fazendo essa verificação da fila certa fo processo aqui entro de scheduler
                // Porque a atividade dizia que os processos sempre iniciavam em ready (fila 1)
                // Porém era melhor fazer isso depois de criar os processos, porque assim exitaria processamente desnecessário
                // E daria para visualizar/debugar melhor
                if (current->process_time_total > tempoLimite) {
                    current->queue = 0;
                    enqueue(ready, current);
                } else {
                    current->queue = 1;
                    enqueue(ready2, current);
                }
                break;
            case BLOCKED:
                enqueue(blocked, current);
                break;
            case FINISHED:
                enqueue(finished, current);
                break;
            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
                break;
        }
    }

    if (isempty(ready) && isempty(ready2))
        return NULL;

    struct queue * targetQueue = NULL;
    int queueCode = 0;

    
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

    // Usei a função abaixo para debug, criei ela no arquivo queue.c
    // print_simple_queue(ready);
    // print_simple_queue(ready2);

    selected = dequeue(targetQueue);

    selected->state = RUNNING;
    selected->queue = queueCode;

    return selected;
}

