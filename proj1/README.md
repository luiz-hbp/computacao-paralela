Projeto realizado por:

<h2>Arthur de Pina Balduino Leitão - 10400677

Luiz Henrique Bonilha Pasquinelli - 10401415</h2>

<p>Dado o problema da estimativa do valor do número de Euler, a primeira solução pensada foi pensando num código serial capaz de calcular o maior número de casas decimais do número de taylor precisamente.</p>

<p>Sem o uso de bibliotecas extras, o primeiro tipo de variável do C que foi utilizado foi o long double, assim foi pensado o código mais primordial do projeto o <b>taylorSerial.c</b> que utilizando um loop acumula o valor dos fatoriais em uma variável e calcula a soma total das sequências. Compilado no tradicional <u>gcc -o taylorSerial taylorSerial.c</u>  e executado com  <u>./taylorSerial.</u></p>

<p>Nessa primeira versão foi possível alcançar 63 casas decimais para o número de Euler : 2.71828182845904523542816810799394033892895095050334930419921875 </p>


![image](https://github.com/luiz-hbp/computacao-paralela/assets/144058147/b7adc350-6e25-4ab4-b100-d9c373a26f8c)


<p>Pensando em possíveis paralelismos para esse código serial, foi concluído que poderíamos distribuir as divisões de fatoriais entre múltiplas threads, usando a biblioteca pthread, e somente depois realizar a soma para os resultados parciais obtidos. Porém esse método não funcionaria com a função projetada de acumular os fatoriais em uma mesma variável, já que as threads responsáveis pelas últimas divisões ficariam ociosas enquanto seus fatoriais não fossem calculadas.</p>

<p>Para contornar isso foi pensado em um meio termo em que cada thread fica responsável por uma sequência de divisões e dentro dessas sequências a soma parcial é feita da mesma forma que o cálculo serial, porém cada começo de sequência, ou seja para a primeira divisão da thread, o fatorial é calculado por uma função iterativa.</p>

<p>A versão paralela do código <b>taylorParalelo.c</b> foi compilada usando gcc -o taylorParalelo taylorParalelo .c -pthread e executado com  ./taylorParalelo N em que N é o número de threads desejada a ser utilizada.
</p>

<p>A versão paralela não alterou o valor de Euler calculado pela versão serial.</p>

![image](https://github.com/luiz-hbp/computacao-paralela/assets/144058147/a345b0c0-5550-41e4-b80f-280ac544ab6e)

<p>Quanto ao tempo de execução, a princípio pareceu que a versão paralela não representava ganho de performance como mostram os gráficos a seguir:</p>

<b>Gráfico do Tempo de Execução em ms X N processadores</b>


![image](https://github.com/luiz-hbp/computacao-paralela/assets/144058147/a2d9d9bf-9c57-4653-a5d5-a408ce892408)


<b>Gráfico de Speedup X N processadores</b>


![image](https://github.com/luiz-hbp/computacao-paralela/assets/144058147/37d38140-c685-411b-8f65-088711ee885c)

<p>Porém na primeira estimativa estávamos utilizando poucas divisões, ao aumentar o número de frações da sequência de taylor para 10000000, o ganho de performance da versão paralela ficou mais evidente.
</p>

<b>Gráfico do Tempo de Execução em s X N processadores</b>


![image](https://github.com/luiz-hbp/computacao-paralela/assets/144058147/d162baa6-54ee-44aa-a03d-e6af3bf1e990)


<b>Gráfico de Speedup X N processadores</b>


![image](https://github.com/luiz-hbp/computacao-paralela/assets/144058147/f47e0b58-7759-45f8-8bca-3460957b257b)


<p>O uso do tipo long double diminuía muito nossas possibilidades de aumentar o número de casas decimais do valor de Euler calculado, para isso foi criada uma nova versão do <b>taylorParalelo.c</b> chamada <b>taylorMpfr.c</b>, a biblioteca mpfr possibilitou o aumento da precisão dos decimais de Euler.

Usando apt install libgmp-dev seguido apt install libmpfr-dev é possível usar essa biblioteca de alta precisão decimal, o código do <b>taylorParalelo.c</b> foi adaptado substituindo as operações aritméticas padrão pelas funções da biblioteca mpfr como: mpfr_add e mpfr_div Para Compilar essa versão usa-se gcc -o taylorMpfr taylorMpfr.c -lmpfr -lgmp -pthread</p>


![image](https://github.com/luiz-hbp/computacao-paralela/assets/144058147/cf9430d4-080e-4ecb-bf80-81651c954e7a)


<p>Com a versão taylorMpfr.c foi possível alcançar 301030 casas decimais como pode ser visto no arquivo euler.txt</p>


<p>Quanto a performance desse algoritmo mais robusto em calcular casas decimais, os tempos de execução por número de processadores usados ficaram assim</p>

![image](https://github.com/luiz-hbp/computacao-paralela/assets/144058147/0cedf66d-e7e3-40df-ad4f-2a8e6cb57960)


Todas as execuções foram realizadas no processador com as seguintes especificações:

processor       : 0

vendor_id       : GenuineIntel

cpu family      : 6

model           : 63

model name      : Intel(R) Xeon(R) CPU E5-2676 v3 @ 2.40GHz

stepping        : 2

microcode       : 0x49cpu

MHz         : 2399.956

cache size      : 30720 KB

physical id     : 0

siblings        : 1

core id         : 0

cpu cores       : 1

apicid          : 0

initial apicid  : 0

fpu             : yes

fpu_exception   : yes

cpuid level     : 13

wp              : yes

flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx rdtscp lm constant_tsc rep_good nopl xtopology cpuid tsc_known_freq pni pclmulqdq ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand hypervisor lahf_lm abm cpuid_fault invpcid_single pti fsgsbase bmi1 avx2 smep bmi2 erms invpcid xsaveopt

bugs            : cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf mds swapgs itlb_multihit mmio_stale_data

bogomips        : 4800.0

clflush size    : 64

cache_alignment : 64

address sizes   : 46 bits physical, 48 bits virtual

