# Ndn-cxx_security_tutorial 

by Mateus Sousa (UFBA)

## Configurando ambiente docker 
* Foi criada uma imagem personalizada da biblioteca ndn-cxx. Nessa imagem estão isntaladas as libs ndn-cxx e NFD prontas para uso. 
* Neste tutorial será abordado a validação de conteúdos em Named Data Networking (NDN). 

### Pré-requisitos 
* Docker devidamente instalado e configurado (ver https://www.docker.com/)
* Baixe a imagem: ```docker pull mateussousa/ndn```
## Montagem da rede 
* Para criar uma rede para os _containers_ execute o comando: ```docker network create -d bridge ndn```
* Uma vez criada a rede, é possível subir os _containers_, em terminais diferentes execute: 
- docker run --name consumer --network ndn -it mateussousa/ndn bash
- docker run --name producer --network ndn -it mateussousa/ndn bash
- docker run --name router --network ndn -it mateussousa/ndn bash

## Configurando segurança
No produtor você irá criar as chaves e o certificado raíz. 
- ndnsec-key-gen /example/ > root.key
- ndnsec-cert-dump -i /example/ > root.cert
- ndnsec-cert-install -f root.cert

Após isso, copie o root.cert para o consumidor
- No consumidor, execute ```mkdir -p /usr/local/etc/ndn/keys ```
- copie o cert para esse dir. 
- copie o arquivo localizado em [rules/](rules/custom_rule.txt) para o diretório ndn-cxx/ dentro do container. 
- Por fim, copie os aquivos [consumer.cpp](apps/consumer.cpp) e [producer.cpp](apps/producer.cpp) para o diretório ndn-cxx/examples nos containers consumer e producer, respectivamente. 

- Em cada container execute o ./waf para compilar as aplicações.




## Setting up
### Instalação das libs ndn-cxx e NFD no host (versão utilizada Ubuntu 18.04)
* [ndn-cxx](https://github.com/named-data/ndn-cxx)
  - ``` sudo apt install g++ pkg-config python3-minimal libboost-all-dev libssl-dev libsqlite3-dev ```
  - ``` sudo apt install doxygen graphviz python3-pip ```
  - ``` sudo pip3 install sphinx sphinxcontrib-doxylink ```
  - ``` git clone https://github.com/named-data/ndn-cxx ```
  - ``` git clone --recursive https://github.com/named-data/NFD.git ```

* Building
- ``` cd ndn-cxx && ./waf configure --with-examples && ./waf && sudo ./waf install ```
- Saia do diretório ndn-cxx
- ``` cd NFD && ./waf configure && ./waf && sudo  ./waf install ; ldconfig ```
- Adicione o arquivo de configuração default do NFD
- ```sudo cp /usr/local/etc/ndn/nfd.conf.sample /usr/local/etc/ndn/nfd.conf ```

### Execução do NFD
* Para inicializar o daemon execute 
- ```nfd-start```
- Cheque o status em seguida com o comando:
- ```ndn-status```

