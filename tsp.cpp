#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;

int ** criarVetor2D(int tamanho);
vector<int> vizinho(int **matriz, int tamanho);
vector<int> swap(int **matriz, int tamanho, vector<int> vizinho);
int calculaCusto(int **matriz, vector<int> caminho);
void imprimeSolucao(vector<int> solucao);
//int opt2(vector<int> solucao, int **matriz, int tamanho, int custo);
//vector<int> opt2Swap(vector<int> solucao, int limite1, int limite2);
vector<int>  opt2(int **matriz, int tamanho, vector<int> vizinho);
void vnd(vector <int> solucao, int **matriz, int tamanho);

int main(){ 
	int tamanho = 0;
	int limite1, limite2;
	//Leitura do arquivo
	ifstream grafo_matriz("pcv10.txt", ios::in);
	grafo_matriz >> tamanho; 
	int **matriz = criarVetor2D(tamanho);

	for (int i = 0; i < tamanho; i++){
		for (int j = 0; j < tamanho; j++){
			grafo_matriz >> matriz[i][j];
		}
	}
	//fim letura do arquivo

	auto solucao_inicial = vizinho(matriz, tamanho); //Solução inicial gerada com o algortimo do vizinho mais proximo
	cout << "Solucao Inicial: "; imprimeSolucao(solucao_inicial);
	vnd(solucao_inicial, matriz, tamanho); //Busca Local

	return 0;
}

//Heuristica construtiva
vector<int> vizinho(int **matriz, int tamanho){

	int aux = 0;
	int custo_final = 0;
	vector <int> solucao;
	int verificado[tamanho] = {}; 
	solucao.push_back(0); 
	int caminho[tamanho] = {};

	caminho[0] = 0;
	verificado[0] = 1;


	for(int i = 0; i < tamanho; i++){
		int custo = INT_MAX;
		int indice = 0;
		for(int j = 0; j < tamanho; j++){
			if(!verificado[j] && custo > matriz[i][j]){
				indice = j;
				custo = matriz[i][j];
			}
		}

		caminho[i + 1] = indice;
		verificado[indice] = 1;
		solucao.push_back(indice);
		
	}

	caminho[tamanho] = 0;
	solucao.push_back(0);

	for (int i = 0; i < tamanho; i++)
		custo_final += matriz[caminho[i]][caminho[i+1]];

	
	cout << "Custo " << custo_final << endl;
	return solucao;
}

//Movimento de vizinhaça 1
vector<int> swap(int **matriz, int tamanho, vector<int> vizinho){
	vector<int> temporario;
	temporario = vizinho;
	int custo_primario = calculaCusto(matriz, vizinho);
	int custo_temporario;
	for(int i = 1; i < tamanho; i++){
		temporario = vizinho;

		for(int j = i + 1; j < tamanho; j++){
			int aux = temporario[i];
			temporario[i] = temporario[j];
			temporario[j] = aux;

			custo_temporario = calculaCusto(matriz, temporario);

			if(custo_temporario < custo_primario){
				custo_primario = custo_temporario;
				vizinho = temporario;
				custo_temporario = calculaCusto(matriz, vizinho);
			}
		}
	}

return vizinho;
	
}

//Movimento de vizinhança 2
vector <int> opt2(int **matriz, int tamanho, vector<int> vizinho){
	vector <int> temporario;
	int custo_primario = calculaCusto(matriz, vizinho);
	int custo_temporario;
	temporario = vizinho;
	for(int j = 1; j < tamanho/2; j++){
		for(int i = j; i < tamanho/2; i++){
			
			int aux = temporario[tamanho - i];
			temporario[tamanho - i] = temporario[i];
			temporario[i] = aux;
			
			custo_temporario = calculaCusto(matriz, temporario);
			if(custo_temporario < custo_primario){
				custo_primario = custo_temporario;
				vizinho = temporario;
			}
			
		}
	}

	return vizinho;
}


/*
vector<int> opt2Swap(vector <int> solucao, int limite1, int limite2){
	vector<int> novarota;
	novarota = solucao;
	for(int i = limite2; i > limite1; i--){
		novarota.push_back(solucao[i]);
		cout << i;
	}

	return novarota;
}

int opt2(vector<int> solucao, int ** matriz, int tamanho, int custo){
	vector <int> novarota;
	vector <int> solucao_atual;
	int melhor_custo = custo;
	int novo_custo;
	cout << melhor_custo;

	for(int i = 1; i < tamanho ; i++){
		for(int k = i + 1; k < tamanho; i++){
			novarota = opt2Swap(solucao, i, k);
			novo_custo = calculaCusto(matriz, novarota);
			if(novo_custo < melhor_custo){
				solucao_atual = novarota;
				melhor_custo = novo_custo;
			}
		}
	}

	return melhor_custo;
}
*/

//Busca Local
void vnd(vector <int> solucao, int **matriz, int tamanho){
	int numEstruturas = 2;
	int i = 1;
	int custo = calculaCusto(matriz, solucao);
	int custo_atual;
	vector <int> melhor_solucao;
	melhor_solucao = solucao;

	while( i <= numEstruturas){
		if(i == 1){
			melhor_solucao = swap(matriz, tamanho, solucao);
			custo_atual = calculaCusto(matriz, melhor_solucao);
			custo =  calculaCusto(matriz,solucao);
		}else if( i == 2){
			melhor_solucao = opt2(matriz, tamanho, solucao);
			custo_atual = calculaCusto(matriz, melhor_solucao);
			custo =  calculaCusto(matriz,solucao);
		}

		if(custo_atual < custo){
			solucao = melhor_solucao;
			i = 1;
		}else{
			i++;
		}
	}

	cout << "Melhor solução encontrada com custo: " << custo_atual << endl;
	imprimeSolucao(melhor_solucao);

}
//Calcula custo da solução
int calculaCusto(int **matriz, vector<int> caminho) {
    int custo = 0;

    for(int i = 0; i < caminho.size() - 1; i++) {
        custo = custo + matriz[caminho[i]][caminho[i + 1]];
    }

    return custo;
}

//Cria matriz
int ** criarVetor2D(int tamanho){
	int **a = new int*[tamanho];
	for(int i = 0; i < tamanho; i++)
		a[i] = new int[tamanho];
	for(int i = 0; i < tamanho; i++)
		for(int j = 0; j < tamanho; j++)
			a[i][j] = 0;
	return a;
}

//Exibe na tela a solução
void imprimeSolucao(vector<int> solucao){
	for(int i = 0; i < solucao.size() - 1; i++){
		cout << " " << solucao[i];
	}
	cout << endl;
}






/*
int opt2(vector <int> solucao, int *limite1, int *limite2, int **matriz){
	int troca, trocaux, melhortroca = 0;
	*limite1 = 0;
	*limite2 = 0;
	for(int i = 2; i < solucao.size() - 2; i++){
		troca = matriz[solucao[i]][solucao[i-1]];
		for(int j = i + 4; j < solucao.size() - 2; j++){
			trocaux = troca + matriz[solucao[j]][solucao[j+1]] - (matriz[solucao[i-1]][solucao[j]]+ matriz[solucao[i]][solucao[j+1]]);
			if(trocaux > 0 && trocaux > melhortroca){
				melhortroca = trocaux;
				*limite1 = i;
				*limite2 = j;
			}
		}
	}

	return melhortroca;
}
*/
/*
vector<int> trocaOpt2(vector <int> solucao, int limite1, int limite2){
	int i = limite1;
	int j = limite2;
	int aux = 0;
	cout << "teste";
	if((limite1 - limite2)%2 == 0){
		do{
			aux = solucao[i];
			solucao[i] = solucao[j];
			solucao[j] = aux;
			i++;
			j--;
		}while(i!=j);
	}else{
		do{
			aux = solucao[i];
			solucao[i] = solucao[j];
			solucao[j] = aux;
			i++;
			j--;
		}while(i!= j + 1);
	}

	if(limite1 == 1 || limite2 == solucao.size() - 2){
		solucao[0] = solucao[solucao.size() - 2];
		solucao[solucao.size() - 1] = solucao[1];
	}

	return solucao;
}
*/



/*
for(int i = 0; i < tamanho; i++){
		int custo = INT_MAX;
		int indice = 0;
		for(int j = 0; j < tamanho; j++){
			if(matriz[aux][j] < custo 
				&& matriz[aux][j] != 0
				&& find(solucao.begin(),solucao.end(), j+1) == solucao.end()
				) {
				custo = matriz[aux][j];
				indice = j;
			}
		}
		aux = indice; 
		solucao.push_back(indice+1);
		
	}
*/
