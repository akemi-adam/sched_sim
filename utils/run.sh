#!/bin/bash

# Com base na especificação que o senhor passou no descritivo da atividade
# Orientei o ChatGPT a criar um script shell que realiza a contabilização dos tempos de execução de cada algoritmo
# Seguindo o que está no material. Ele basicamente percorre todas as quantidades de números de processos
# Percorre as variações de quantum e os algoritmos

algoritmos=("fifo" "sjf" "ljf" "prio_static" "prio_dynamic" "prio_dynamic_quantum")

saida="estatisticas_tme.csv"

echo "algoritmo,nproc,quantum,media_tme,desvio_padrao" > "$saida"

AMOSTRAS=30

for alg in "${algoritmos[@]}"
do
    for nproc in 10 20 30 40 50 60 70 80 90 100
    do
        for quantum in 10 20 30
        do
            executavel="./main_$alg"

            soma=0
            soma_quadrados=0

            for i in $(seq "$AMOSTRAS")
            do
                # Aqui ele captura o valor da execução do algoritmo
                valor=$($executavel -n "$nproc" -q "$quantum" | awk '/TME:/ {print $2}')

                if [ -z "$valor" ]; then
                    echo "Erro ao capturar TME de $alg"
                    continue
                fi

                soma=$(echo "$soma + $valor" | bc)
                soma_quadrados=$(echo "$soma_quadrados + ($valor * $valor)" | bc)
            done

            media=$(echo "scale=6; $soma / $AMOSTRAS" | bc)

            variancia=$(echo "scale=6; \
                ($soma_quadrados - (($soma * $soma) / $AMOSTRAS)) / ($AMOSTRAS - 1)" | bc)

            desvio=$(echo "scale=6; sqrt($variancia)" | bc)

            echo "$alg,$nproc,$quantum,$media,$desvio" >> "$saida"

            echo "OK -> $alg n=$nproc q=$quantum"
        done
    done
done