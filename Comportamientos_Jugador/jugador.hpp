#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;
};


class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      last_action = actIDLE;
      //Orientacion = norte;
      current_state.fil = current_state.col = 99; //valor por defecto
      current_state.brujula = norte;
      bien_situado=false;
      girar_derecha = false;
      zapatillas=false;
      recarga=false;
      ir_objeto=false;
      girar_izquierda=false;
      giro=0;
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

  bool girar_derecha;
  bool bien_situado;
  bool zapatillas;
  bool bikini;
  bool recarga;
  bool ir_objeto;
  bool girar_izquierda;
  int giro;

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
        
        /*for(int i=0; i<4; i++){
          for (int j=0; j<16; j++){
            matriz[st.fil+i][st.col] = terreno[j];
            matriz[st.fil+i][st.col] = terreno[j];
            matriz[st.fil+i][st.col] = terreno[j];
          }
        }*/
  
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
    s
  }

  //compruebo las casillas que veo 
  int mirar_terreno(char casilla, const vector<unsigned char> &terreno, const state &st){
    bool encontrado=false;
    int donde=-1;
    for (int i=2;  i<=16; i++){
      if (terreno[i]==casilla){
        encontrado=true;
        donde=i; 
      }
    }
    return donde;
  }

  Action coger_objetos(const vector<unsigned char> &terreno, const state &st){
    Action accion_a_realizar;
    int donde_zapatillas=mirar_terreno('D',terreno, st);
    int donde_bikini=mirar_terreno('K',terreno, st);
    int donde_recarga=mirar_terreno('X',terreno, st);

    if (donde_zapatillas>0 and zapatillas==false){ //miro a ver si hay zapatillas
      if (terreno[donde_zapatillas]==2 or terreno[donde_zapatillas]==6 or terreno[donde_zapatillas]==12)
        accion_a_realizar=actFORWARD;
      if ((terreno[donde_zapatillas]==1 or terreno[donde_zapatillas]==5 or terreno[donde_zapatillas]==11) and (st.brujula==norte or st.brujula==sur or st.brujula==este or st.brujula==oeste) )
        accion_a_realizar=actTURN_SL; 
      else
        accion_a_realizar=actFORWARD; 
      if ((terreno[donde_zapatillas]==3 or terreno[donde_zapatillas]==7 or terreno[donde_zapatillas]==8) and (st.brujula==norte or st.brujula==sur or st.brujula==este or st.brujula==oeste))
        accion_a_realizar=actTURN_SR; 
      else 
        accion_a_realizar=actFORWARD; 
    }

    if (donde_bikini>0 and bikini==false){ //miro a ver si hay zapatillas
      if (terreno[donde_bikini]==2 or terreno[donde_bikini]==6 or terreno[donde_bikini]==12)
        accion_a_realizar=actFORWARD;
      if ((terreno[donde_bikini]==1 or terreno[donde_bikini]==5 or terreno[donde_bikini]==11) and (st.brujula==norte or st.brujula==sur or st.brujula==este or st.brujula==oeste) )
        accion_a_realizar=actTURN_SL; 
      else
        accion_a_realizar=actFORWARD; 
      if ((terreno[donde_bikini]==3 or terreno[donde_bikini]==7 or terreno[donde_bikini]==8) and (st.brujula==norte or st.brujula==sur or st.brujula==este or st.brujula==oeste))
        accion_a_realizar=actTURN_SR; 
      else 
        accion_a_realizar=actFORWARD; 
      
    }

    if (donde_recarga>0 and recarga==false){ //miro a ver si hay zapatillas
      if (terreno[donde_recarga]==2 or terreno[donde_recarga]==6 or terreno[donde_recarga]==12)
        accion_a_realizar=actFORWARD;
      if ((terreno[donde_recarga]==1 or terreno[donde_recarga]==5 or terreno[donde_recarga]==11) and (st.brujula==norte or st.brujula==sur or st.brujula==este or st.brujula==oeste) )
        accion_a_realizar=actTURN_SL; 
      else
        accion_a_realizar=actFORWARD; 
      if ((terreno[donde_recarga]==3 or terreno[donde_recarga]==7 or terreno[donde_recarga]==8) and (st.brujula==norte or st.brujula==sur or st.brujula==este or st.brujula==oeste))
        accion_a_realizar=actTURN_SR; 
      else 
        accion_a_realizar=actFORWARD; 
      
    }
    return accion_a_realizar; 
  }

  //Action giro_random(){}
};

#endif
