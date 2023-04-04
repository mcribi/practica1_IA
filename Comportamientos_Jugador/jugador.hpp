#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <cstdlib>
#include <ostream>
#include <iostream>
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;
};


class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){ // Constructor de la clase
      
      // Dar el valor inicial a las variables de estado
      last_action = actIDLE;
      //Orientacion = norte;
      current_state.fil = current_state.col = size; //valor por defecto
      current_state.brujula = norte;
      bien_situado=false;
      zapatillas=false;
      bikini=false;
      recarga=false;
      int fila_ultima=current_state.fil;
	    int col_ultima=current_state.col;
      ngiros=0;
      ciclos=2999;
      reset=false;
      he_girado=false;
      ultima_recarga=2999;

      //Inicialzamos la matriz de numero de veces pasada por cada casilla
      for (int i=0; i<size; i++){
        vector <int> fil; 
        for (int j=0; j<size; j++){
          fil.push_back(0);
        }
        nveces.push_back(fil);
      }
       //Inicialzamos la matriz de numero de veces pasada por cada casilla
      for (int i=0; i<2*size; i++){
        vector <int> fil; 
        for (int j=0; j<2*size; j++){
          fil.push_back(0);
        }
        nveces_aprox.push_back(fil);
      }

      //inicializamos la matriz que rellenamos antes de estar bien ubicados
      for (int i=0; i<2*size; i++){
        vector <unsigned char> fila; 
        for (int j=0; j<2*size; j++){
          fila.push_back('?');
        }
        matriz_aprox.push_back(fila);
      }

      //Sabemos que hay 3x3 de precipicio
      for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
          if (i<3 or j<3 or j>(size-4) or i>(size-4))
            mapaResultado[i][j]='P';
        }
      }


    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  
  // Declarar aquí las variables de estado
  Action last_action;
  //Orientacion brujula;

  state current_state;

  
  bool bien_situado;
  bool zapatillas;
  bool bikini;
  bool recarga;
  int ngiros;
  vector<vector< int>> nveces;
  vector<vector< int>> nveces_aprox;
  vector<vector< unsigned char>> matriz_aprox;
  int ciclos;
  bool reset;
  bool he_girado;
  int ultima_recarga;


  void TransladarMatriz(const state &st, vector<vector<unsigned char>>&matriz_real, vector<vector<unsigned char>>&matriz_inventada, int fil_ult, int col_ult){
      for (int i=0; i<matriz_real.size(); i++){
        for (int j=0; j<matriz_real.size(); j++){
            if(matriz_real[i][j]=='?' && matriz_inventada[fil_ult-st.fil+i][col_ult-st.col+j]!='?'){
              int x = fil_ult-st.fil+i;
              int y = col_ult-st.col+j;
              matriz_real[i][j] = matriz_inventada[x][y];
            }
        }
      }
  }

  void TransladarMatrizInt(const state &st, vector<vector<int>>&matriz_real, vector<vector<int>>&matriz_inventada, int fil_ult, int col_ult){
      for (int i=0; i<matriz_real.size(); i++){
        for (int j=0; j<matriz_real.size(); j++){
            if(matriz_real[i][j]==0 && matriz_inventada[fil_ult-st.fil+i][col_ult-st.col+j]!=0){
              int x = fil_ult-st.fil+i;
              int y = col_ult-st.col+j;
              matriz_real[i][j] = matriz_inventada[x][y];
            }
        }
      }
  }

  bool ComprobarNVeces(const state &st, vector<vector<int>>&matriz_real, vector<vector<int>>&matriz_inventada){
      bool comprobar=false;

      if (bien_situado==true){
        if (matriz_real[st.fil][st.col]>4)
          comprobar=true;
      }else{ //esta mal situado
        if (matriz_inventada[st.fil][st.col]>4)
          comprobar=true;
      }

      return comprobar;
  }

  void PonerTerrenoEnMatriz (const vector<unsigned char> &terreno, const state &st, vector<vector<unsigned char>>&matriz){
    //Actualiza el mapa con la casilla donde se encuentre el agente
    matriz[st.fil][st.col] = terreno[0];
    if ((current_state.brujula==norte) or (current_state.brujula==sur) or (current_state.brujula==este) or (current_state.brujula==oeste))
      if (current_state.brujula==sur){
        matriz[st.fil][st.col] = terreno[0];

        matriz[st.fil+1][st.col+1] = terreno[1];
        matriz[st.fil+1][st.col] = terreno[2];
        matriz[st.fil+1][st.col-1] = terreno[3];

        matriz[st.fil+2][st.col+2] = terreno[4];
        matriz[st.fil+2][st.col+1] = terreno[5];
        matriz[st.fil+2][st.col] = terreno[6];
        matriz[st.fil+2][st.col-1] = terreno[7];
        matriz[st.fil+2][st.col-2] = terreno[8];

        matriz[st.fil+3][st.col+3] = terreno[9];
        matriz[st.fil+3][st.col+2] = terreno[10];
        matriz[st.fil+3][st.col+1] = terreno[11];
        matriz[st.fil+3][st.col] = terreno[12];
        matriz[st.fil+3][st.col-1] = terreno[13];
        matriz[st.fil+3][st.col-2] = terreno[14];
        matriz[st.fil+3][st.col-3] = terreno[15];  
      }

      if (current_state.brujula==norte){
        matriz[st.fil][st.col] = terreno[0];

        matriz[st.fil-1][st.col-1] = terreno[1];
        matriz[st.fil-1][st.col] = terreno[2];
        matriz[st.fil-1][st.col+1] = terreno[3];

        matriz[st.fil-2][st.col-2] = terreno[4];
        matriz[st.fil-2][st.col-1] = terreno[5];
        matriz[st.fil-2][st.col] = terreno[6];
        matriz[st.fil-2][st.col+1] = terreno[7];
        matriz[st.fil-2][st.col+2] = terreno[8];

        matriz[st.fil-3][st.col-3] = terreno[9];
        matriz[st.fil-3][st.col-2] = terreno[10];
        matriz[st.fil-3][st.col-1] = terreno[11];
        matriz[st.fil-3][st.col] = terreno[12];
        matriz[st.fil-3][st.col+1] = terreno[13];
        matriz[st.fil-3][st.col+2] = terreno[14];
        matriz[st.fil-3][st.col+3] = terreno[15];
      }

      if (current_state.brujula==este){
        matriz[st.fil][st.col] = terreno[0];

        matriz[st.fil-1][st.col+1] = terreno[1];
        matriz[st.fil][st.col+1] = terreno[2];
        matriz[st.fil+1][st.col+1] = terreno[3];

        matriz[st.fil-2][st.col+2] = terreno[4];
        matriz[st.fil-1][st.col+2] = terreno[5];
        matriz[st.fil][st.col+2] = terreno[6];
        matriz[st.fil+1][st.col+2] = terreno[7];
        matriz[st.fil+2][st.col+2] = terreno[8];

        matriz[st.fil-3][st.col+3] = terreno[9];
        matriz[st.fil-2][st.col+3] = terreno[10];
        matriz[st.fil-1][st.col+3] = terreno[11];
        matriz[st.fil][st.col+3] = terreno[12];
        matriz[st.fil+1][st.col+3] = terreno[13];
        matriz[st.fil+2][st.col+3] = terreno[14];
        matriz[st.fil+3][st.col+3] = terreno[15];
      }

      if (current_state.brujula==oeste){
        matriz[st.fil][st.col] = terreno[0];

        matriz[st.fil+1][st.col-1] = terreno[1];
        matriz[st.fil][st.col-1] = terreno[2];
        matriz[st.fil-1][st.col-1] = terreno[3];

        matriz[st.fil+2][st.col-2] = terreno[4];
        matriz[st.fil+1][st.col-2] = terreno[5];
        matriz[st.fil][st.col-2] = terreno[6];
        matriz[st.fil-1][st.col-2] = terreno[7];
        matriz[st.fil-2][st.col-2] = terreno[8];

        matriz[st.fil+3][st.col-3] = terreno[9];
        matriz[st.fil+2][st.col-3] = terreno[10];
        matriz[st.fil+1][st.col-3] = terreno[11];
        matriz[st.fil][st.col-3] = terreno[12];
        matriz[st.fil-1][st.col-3] = terreno[13];
        matriz[st.fil-2][st.col-3] = terreno[14];
        matriz[st.fil-3][st.col-3] = terreno[15];
      }

      if (current_state.brujula==noreste){
        matriz[st.fil][st.col] = terreno[0];

        matriz[st.fil-1][st.col] = terreno[1];
        matriz[st.fil-1][st.col+1] = terreno[2];
        matriz[st.fil][st.col+1] = terreno[3];

        matriz[st.fil-2][st.col] = terreno[4];
        matriz[st.fil-2][st.col+1] = terreno[5];
        matriz[st.fil-2][st.col+2] = terreno[6];
        matriz[st.fil-1][st.col+2] = terreno[7];
        matriz[st.fil][st.col+2] = terreno[8];

        matriz[st.fil-3][st.col] = terreno[9];
        matriz[st.fil-3][st.col+1] = terreno[10];
        matriz[st.fil-3][st.col+2] = terreno[11];
        matriz[st.fil-3][st.col+3] = terreno[12];
        matriz[st.fil-2][st.col+3] = terreno[13];
        matriz[st.fil-1][st.col+3] = terreno[14];
        matriz[st.fil][st.col+3] = terreno[15];
      }

      if (current_state.brujula==noroeste){
        matriz[st.fil][st.col] = terreno[0];

        matriz[st.fil][st.col-1] = terreno[1];
        matriz[st.fil-1][st.col-1] = terreno[2];
        matriz[st.fil-1][st.col] = terreno[3];

        matriz[st.fil][st.col-2] = terreno[4];
        matriz[st.fil-1][st.col-2] = terreno[5];
        matriz[st.fil-2][st.col-2] = terreno[6];
        matriz[st.fil-2][st.col-1] = terreno[7];
        matriz[st.fil-2][st.col] = terreno[8];

        matriz[st.fil][st.col-3] = terreno[9];
        matriz[st.fil-1][st.col-3] = terreno[10];
        matriz[st.fil-2][st.col-3] = terreno[11];
        matriz[st.fil-3][st.col-3] = terreno[12];
        matriz[st.fil-3][st.col-2] = terreno[13];
        matriz[st.fil-3][st.col-1] = terreno[14];
        matriz[st.fil-3][st.col] = terreno[15];
      }

      if (current_state.brujula==suroeste){
        matriz[st.fil][st.col] = terreno[0];

        matriz[st.fil+1][st.col] = terreno[1];
        matriz[st.fil+1][st.col-1] = terreno[2];
        matriz[st.fil][st.col-1] = terreno[3];

        matriz[st.fil+2][st.col] = terreno[4];
        matriz[st.fil+2][st.col-1] = terreno[5];
        matriz[st.fil+2][st.col-2] = terreno[6];
        matriz[st.fil+1][st.col-2] = terreno[7];
        matriz[st.fil][st.col-2] = terreno[8];

        matriz[st.fil+3][st.col] = terreno[9];
        matriz[st.fil+3][st.col-1] = terreno[10];
        matriz[st.fil+3][st.col-2] = terreno[11];
        matriz[st.fil+3][st.col-3] = terreno[12];
        matriz[st.fil+2][st.col-3] = terreno[13];
        matriz[st.fil+1][st.col-3] = terreno[14];
        matriz[st.fil][st.col-3] = terreno[15];
      }

      if (current_state.brujula==sureste){
        matriz[st.fil][st.col] = terreno[0];

        matriz[st.fil][st.col+1] = terreno[1];
        matriz[st.fil+1][st.col+1] = terreno[2];
        matriz[st.fil+1][st.col] = terreno[3];

        matriz[st.fil][st.col+2] = terreno[4];
        matriz[st.fil+1][st.col+2] = terreno[5];
        matriz[st.fil+2][st.col+2] = terreno[6];
        matriz[st.fil+2][st.col+1] = terreno[7];
        matriz[st.fil+2][st.col] = terreno[8];

        matriz[st.fil][st.col+3] = terreno[9];
        matriz[st.fil+1][st.col+3] = terreno[10];
        matriz[st.fil+2][st.col+3] = terreno[11];
        matriz[st.fil+3][st.col+3] = terreno[12];
        matriz[st.fil+3][st.col+2] = terreno[13];
        matriz[st.fil+3][st.col+1] = terreno[14];
        matriz[st.fil+3][st.col] = terreno[15];
      }
    
  }

  //compruebo las casillas que veo 
  int mirar_terreno(char casilla, const vector<unsigned char> &terreno){
    bool encontrado=false;
    int donde=-1;
    int i=1;
    while (i<=16 && !encontrado){
      i++;
      if (terreno[i]==casilla){
        encontrado=true;
        donde=i;
      }
    }
    return donde;
    cout<<"he visto en"<<donde<<endl; 
  }

  Action coger_bikini(const vector<unsigned char> &terreno, const state &st){
    Action accion_a_realizar;
    int donde_bikini=mirar_terreno('K',terreno);
    bool he_visto_linea_recta=false;


    if (donde_bikini>0){ //miro a ver si hay zapatillas
      if (he_visto_linea_recta){
        accion_a_realizar=actFORWARD;
      }else if (donde_bikini==2){
        accion_a_realizar=actFORWARD;
        bikini=true;
      }else if ( donde_bikini==6 or donde_bikini==12 or donde_bikini==5 or donde_bikini==10 or donde_bikini==11 or donde_bikini==7 or donde_bikini==8 or donde_bikini==13 or donde_bikini==14 ){
        accion_a_realizar=actFORWARD;
      }else if ((donde_bikini==1 or donde_bikini==4 or donde_bikini==9 )){
        accion_a_realizar=actTURN_SL; 
        he_visto_linea_recta=true;
      }else if (donde_bikini==3 or donde_bikini==8 or donde_bikini==15 )
      {  
        accion_a_realizar=actTURN_SR;
        he_visto_linea_recta=true;
      }else 
        accion_a_realizar=actFORWARD;
    }
    return accion_a_realizar; 
  }

  Action coger_zapatillas(const vector<unsigned char> &terreno, const state &st){
    Action accion_a_realizar;
    int donde_zapatillas=mirar_terreno('D',terreno);
    bool he_visto_linea_recta=false;

    if (donde_zapatillas>0){ //miro a ver si hay zapatillas
      if (he_visto_linea_recta){
        accion_a_realizar=actFORWARD;
      }else if (donde_zapatillas==2){
        accion_a_realizar=actFORWARD;
        zapatillas=true;
      }else if (donde_zapatillas==6 or donde_zapatillas==12 or donde_zapatillas==5 or donde_zapatillas==10 or donde_zapatillas==11 or donde_zapatillas==7 or donde_zapatillas==8 or donde_zapatillas==13 or donde_zapatillas==14 ){
        accion_a_realizar=actFORWARD;
      }else if ((donde_zapatillas==1 or donde_zapatillas==4 or donde_zapatillas==9 )){
        accion_a_realizar=actTURN_SL; 
        he_visto_linea_recta=true;
      }else if (donde_zapatillas==3 or donde_zapatillas==8 or donde_zapatillas==15 )
      {  
        accion_a_realizar=actTURN_SR;
        he_visto_linea_recta=true;
      }else 
        accion_a_realizar=actFORWARD;
    
    return accion_a_realizar; 
    }

    return accion_a_realizar; 
  }

  Action coger_recarga(const vector<unsigned char> &terreno, const state &st){
    Action accion_a_realizar;
    int donde_recarga=mirar_terreno('X',terreno);
    bool he_visto_linea_recta=false;
    
    if (he_visto_linea_recta){
        accion_a_realizar=actFORWARD;
      }else if (donde_recarga==2){ 
        accion_a_realizar=actFORWARD;
        recarga=true;
        ultima_recarga=ciclos;  
      }else if (donde_recarga==6 or donde_recarga==12 or donde_recarga==5 or donde_recarga==10 or donde_recarga==11 or donde_recarga==7 or donde_recarga==8 or donde_recarga==13 or donde_recarga==14 ){
         accion_a_realizar=actFORWARD;
      }else if ((donde_recarga==1 or donde_recarga==4 or donde_recarga==9 )){
        accion_a_realizar=actTURN_SL; 
        he_visto_linea_recta=true;
      }else if (donde_recarga==3 or donde_recarga==8 or donde_recarga==15 )
      {  
        accion_a_realizar=actTURN_SR;
        he_visto_linea_recta=true;
      }else 
        accion_a_realizar=actFORWARD;
    
    
    return accion_a_realizar; 
  }

  Action coger_posicionamiento(const vector<unsigned char> &terreno, const state &st){
    Action accion_a_realizar;
    int donde_pos=mirar_terreno('G',terreno);
    bool he_visto_linea_recta=false;
    
    if (he_visto_linea_recta){
        accion_a_realizar=actFORWARD;
      }else if (donde_pos==2){
        accion_a_realizar=actFORWARD;
        reset =false;
      }else if (donde_pos==6 or donde_pos==12 or donde_pos==5 or donde_pos==10 or donde_pos==11 or donde_pos==7 or donde_pos==8 or donde_pos==13 or donde_pos==14 ){
        accion_a_realizar=actFORWARD;
      }else if ((donde_pos==1 or donde_pos==4 or donde_pos==9 )){
        accion_a_realizar=actTURN_SL; 
        he_visto_linea_recta=true;
      }else if (donde_pos==3 or donde_pos==8 or donde_pos==15 )
      {  
        accion_a_realizar=actTURN_SR;
        he_visto_linea_recta=true;
      }else 
        accion_a_realizar=actFORWARD;
    
    
    return accion_a_realizar; 
  }

  Action cogerAgujero(const vector<unsigned char> &terreno, const state &st){
    Action accion_a_realizar;
    int donde_pos=veAgujero(terreno, st);
    bool he_visto_linea_recta=false;
    
    if (he_visto_linea_recta){
        accion_a_realizar=actFORWARD;
      }else if (donde_pos==2){
        accion_a_realizar=actFORWARD;
        reset =false;
      }else if (donde_pos==6 or donde_pos==12 or donde_pos==5 or donde_pos==10 or donde_pos==11 or donde_pos==7 or donde_pos==8 or donde_pos==13 or donde_pos==14 ){
        accion_a_realizar=actFORWARD;
      }else if ((donde_pos==1 or donde_pos==4 or donde_pos==9 )){
        accion_a_realizar=actTURN_SL; 
        he_visto_linea_recta=true;
      }else if (donde_pos==3 or donde_pos==8 or donde_pos==15 )
      {  
        accion_a_realizar=actTURN_SR;
        he_visto_linea_recta=true;
      }else 
        accion_a_realizar=actFORWARD;
    
    
    return accion_a_realizar; 
  }

  Action giro_random(){
    Action accion; 
    int girar=rand()%4;
    	if (girar==0){
			accion=actTURN_SR;
		}else if (girar==1){
			accion=actTURN_BL;
		}else if (girar==2){
			accion=actTURN_SL;
		}else if (girar==3){
			accion=actTURN_BR;
		}
    return accion; 
  }

  /*accion seguirMuro (const vector<unsigned char> &terreno, const state &st){
    Action accion;


    return accion;
  }*/
  
  int veAgujero(const vector<unsigned char> &terreno, const state &st){
    //Action accion;
    int donde=-1;
    /*for (int i=0; i<16; i++){
      if ((terreno[i]=='M' or terreno[i]=='P') and (terreno[i+2]=='M' or terreno[i+2]=='P') and (terreno[i+1]!='M' and terreno[i+1]!='P') and current_state.brujula==norte and current_state.brujula==sur and current_state.brujula==este and current_state.brujula==oeste){
        donde=i+1;
      }
    }*/
    //if (st.brujula==norte or st.brujula==sur or st.brujula==este or st.brujula==oeste){
    if (terreno[1]=='P' and terreno[3]=='P' and terreno[2]!='P')
      donde=2;
    else if (terreno[5]=='P' and terreno[7]=='P' and terreno[6]!='P')
      donde=6;
    else if (terreno[4]=='P' and terreno[6]=='P' and terreno[5]!='P')
      donde=5;
    else if (terreno[6]=='P' and terreno[8]=='P' and terreno[7]!='P')
      donde=7;
    else if (terreno[9]=='P' and terreno[11]=='P' and terreno[10]!='P')
      donde=10;
    else if (terreno[10]=='P' and terreno[12]=='P' and terreno[11]!='P')
      donde=11;
    else if (terreno[11]=='P' and terreno[13]=='P' and terreno[12]!='P')
      donde=12;
    else if (terreno[12]=='P' and terreno[14]=='P' and terreno[13]!='P')
      donde=13;
    else if (terreno[13]=='P' and terreno[15]=='P' and terreno[14]!='P')
      donde=14;
    else if (terreno[1]=='M' and terreno[3]=='M' and terreno[2]!='M')
      donde=2;
    else if (terreno[5]=='M' and terreno[7]=='M' and terreno[6]!='M')
      donde=6;
    else if (terreno[4]=='M' and terreno[6]=='M' and terreno[5]!='M')
      donde=5;
    else if (terreno[6]=='M' and terreno[8]=='M' and terreno[7]!='M')
      donde=7;
    else if (terreno[9]=='M' and terreno[11]=='M' and terreno[10]!='M')
      donde=10;
    else if (terreno[10]=='M' and terreno[12]=='M' and terreno[11]!='M')
      donde=11;
    else if (terreno[11]=='M' and terreno[13]=='M' and terreno[12]!='M')
      donde=12;
    else if (terreno[12]=='M' and terreno[14]=='M' and terreno[13]!='M')
      donde=13;
    else if (terreno[13]=='M' and terreno[15]=='M' and terreno[14]!='M')
      donde=14;
    else if (terreno[3]=='M' and terreno[13]=='M' and terreno[7]!='M')
      donde=7;
    else if (terreno[1]=='M' and terreno[11]=='M' and terreno[5]!='M')
      donde=5;
    //}

    return donde;
  }
  

};

#endif
