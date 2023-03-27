#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este" << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur " << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste" << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;


	// Determinar el efecto de la ultima accion enviada
	int a;
	switch(last_action){
		case actFORWARD:
			switch(current_state.brujula){
				case norte: current_state.fil--; break;
				case noreste: current_state.fil--; current_state.col++; break;
				case este: current_state.col++; break;
				case sureste: current_state.fil++; current_state.col++; break;
				case sur: current_state.fil++; break;
				case suroeste: current_state.fil++; current_state.col--; break;
				case oeste: current_state.col--; break;
				case noroeste: current_state.fil--; current_state.col--; break;
			}
			break;
		case actTURN_SL:
			a = current_state.brujula;
			a = (a+7) % 8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_SR:
		a = current_state.brujula;
			a = (a+1) % 8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_BL:
		a = current_state.brujula;
			a = (a+5) % 8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		case actTURN_BR:
		a = current_state.brujula;
		a = (a+3) % 8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
	}

	// 4
	if(sensores.terreno[0] == 'G' and !bien_situado){
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	if (sensores.reset){ //si se ha activado el reseteo esta mal situado
		bien_situado=false;
	}

	if(bien_situado){
		PonerTerrenoEnMatriz (sensores.terreno, current_state, mapaResultado);
	}
	
	
	//DISTINGUIMOS CASOS
	/*if (mirar_terreno('X',sensores.terreno, current_state)>0){	//hemos visto un objeto y vamos a por el 
		accion=coger_objetos(sensores.terreno, current_state);
	}else if (mirar_terreno('D',sensores.terreno, current_state)>0)
	{
		accion=coger_objetos(sensores.terreno, current_state);
	}else if (mirar_terreno('K',sensores.terreno, current_state)>0)
	{
		accion=coger_objetos(sensores.terreno, current_state);
	}
	
	else */if((sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or sensores.terreno[2] == 'G') and sensores.superficie[2] == '_'){
		accion = actFORWARD;
	}
	else if (sensores.terreno[2]=='K' or sensores.terreno[2]=='D' or sensores.terreno[2]=='X'){
		if (sensores.terreno[2]=='K'){
    		bikini=true; 
    		accion = actFORWARD;
    	}
		else if (sensores.terreno[2]=='D'){
		zapatillas=true;
		accion = actFORWARD; 
		}
		else if (sensores.terreno[2]=='X'){
		recarga=true; 
		accion = actFORWARD;
		}
	}
	else if (sensores.terreno[2]=='P' or sensores.terreno[2]=='M'){
		//accion=actTURN_BL;
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
	}
	else if (sensores.terreno[2] == 'B' and zapatillas==true){
			accion = actFORWARD;	
	}
	else if (sensores.terreno[2] == 'A' and bikini==true){
			accion = actFORWARD;	
	}
	else if (sensores.terreno[2] == 'B' or sensores.terreno[2] == 'A'){
		//accion=actTURN_BR;
		int giro=rand()%4;
		if (giro==0){
			//giro=(giro+1)%4;
			accion=actTURN_SR;
		}else if (giro==1){
			//giro=(giro+1)%4;
			accion=actTURN_BL;
		}else if (giro==2){
			//giro=(giro+1)%4;
			accion=actTURN_SL;
		}else if (giro==3){
			//giro=(giro+1)%4;
			accion=actTURN_BR;
		}
	}
	
	last_action = accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

/*Action giro_random(){
    Action accion;
    int girar=rand()%4;
    if (girar==0){
			accion=actTURN_SR;
		}else if (girar==1){
			accion=actTURN_SR;
		}else if (girar==2){
			accion=actTURN_SL;
		}else if (girar==3){
			accion=actTURN_SR;
		}
    return accion; 
}*/
