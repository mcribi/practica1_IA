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
		case (actFORWARD):
			if (sensores.colision==false){
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

	//Nivel 1-> escribe en todo momento ya que siempre esta orientado
	if(sensores.posC!=-1 and !bien_situado and sensores.nivel==0){
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	//Nivel 1,2,3
	if(sensores.terreno[0]=='G' and !bien_situado and sensores.nivel!=0){
		//reset=false;
		int fila_ultima=current_state.fil;
		int col_ultima=current_state.col;
		cout<<"Antes: "<<fila_ultima<<","<<col_ultima;
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
		cout<<"Actualizado: "<<current_state.fil<<","<<current_state.col;
		
		TransladarMatriz (current_state, mapaResultado, matriz_aprox, fila_ultima, col_ultima);
		TransladarMatrizInt (current_state, nveces, nveces_aprox, fila_ultima, col_ultima);
	}

	//cout<<"Estoy en: "<<current_state.fil<<","<<current_state.col;
	
	if (sensores.reset){ //si se ha activado el reseteo esta mal situado
		bien_situado=false;

		cout<<"LOBO"<<endl;
		zapatillas=false;
		bikini=false;
		//empieza con los valores por defecto
		current_state.fil = current_state.col = mapaResultado.size(); //valor por defecto
      	current_state.brujula = norte;

		//si ha habido un reinicio, nos volvemos a desorientar y lo que habiamos recorrido se borra
		cout<<"Estoy en: "<<current_state.fil<<","<<current_state.col;
		for (int i=0; i<matriz_aprox.size(); i++){
			for (int j=0; j<matriz_aprox.size(); j++){
				matriz_aprox[i][j]='?';
			}
      	}
	}

	

	if(bien_situado){
		PonerTerrenoEnMatriz (sensores.terreno, current_state, mapaResultado);
		//actualizamos la matriz de nveces
		nveces[current_state.fil][current_state.col]++;
	}else{ 
		PonerTerrenoEnMatriz (sensores.terreno, current_state, matriz_aprox);
		nveces_aprox[current_state.fil][current_state.col]++;
	}

	//DISTINGUIMOS CASOS
	if (sensores.superficie[2]=='a' or sensores.superficie[2]=='l'){
		//accion=giro_random();
		if (bien_situado)
			accion=giro_orientado(sensores.terreno, current_state, nveces); 
		else 
			accion=giro_orientado(sensores.terreno, current_state, nveces_aprox); 
		ngiros++;
	}else if (ve_agujero  and sensores.terreno[2]!='M' and sensores.terreno[2]!='P'){
		cout<<"Antes he hecho la casilla "<<cont<<endl;
		cont++;
		cout<<"Ahora voy a hacer la casilla "<<cont<<endl;
		ngiros=0;
		accion=acciones[cont];
		cout<<"siguiendo ciclo"<<endl;

		if (cont>=3){
			cout<<"Ya he traspasado el agujero"<<endl;
			cont=-1; 
			acciones.clear();
			ve_agujero=false;
			
			if(bien_situado){
				nveces[current_state.fil][current_state.col]--;
			}else{ 
				nveces_aprox[current_state.fil][current_state.col]--;
			}
			
		}
	}else if ((mirar_terreno('G',sensores.terreno)>0) && bien_situado==false && sensores.terreno[2]!='P' && sensores.terreno[2]!='M' and sensores.nivel!=0){	
		cout<<"He visto pos"<<endl; 
		accion=coger_posicionamiento(sensores.terreno, current_state);
		ngiros=0;
	}else if ((mirar_terreno('X',sensores.terreno)>0) && sensores.terreno[2]!='P' && sensores.terreno[2]!='M' and (ultima_recarga-ciclos)>200){	
		recargando=0;
		cout<<"He visto recarguita"<<endl; 
		accion=coger_recarga(sensores.terreno, current_state);
		ngiros=0;
	}else if (sensores.terreno[0]=='X' && sensores.bateria<2275 && ciclos>1000 ){	
		accion=actIDLE; //esperamos a estar cargados para seguir
		/*if (bien_situado)
			accion=giro_orientado(sensores.terreno, current_state, nveces); 
		else 
			accion=giro_orientado(sensores.terreno, current_state, nveces_aprox); 
		
		if (accion==actFORWARD)	//no quiero que salga de la casilla
			accion=actIDLE;*/
		
	}else if (sensores.terreno[0]=='X' and recargando<30  and (sensores.vida-ciclos)<3000){	
		accion=actIDLE; //esperamos a estar cargados para seguir
		/*if (bien_situado)
			accion=giro_orientado(sensores.terreno, current_state, nveces); 
		else 
			accion=giro_orientado(sensores.terreno, current_state, nveces_aprox); 
		
		if (accion==actFORWARD)	//no quiero que salga de la casilla
			accion=actIDLE;
		*/
		recargando++;
	}else if ((mirar_terreno('D',sensores.terreno)>0) && (zapatillas==false) && sensores.terreno[2]!='P' && sensores.terreno[2]!='M') {
		cout<<"He visto zapatillas"<<endl; 
		accion=coger_zapatillas(sensores.terreno, current_state);
	}else if ((mirar_terreno('K',sensores.terreno)>0) && (bikini==false)&& sensores.terreno[2]!='P' && sensores.terreno[2]!='M'){
		cout<<"He visto bikini"<<endl;
		accion=coger_bikini(sensores.terreno, current_state);
	}else if (veAgujero(sensores.terreno, current_state)>0 and sensores.terreno[2]!='M' and sensores.terreno[2]!='P'){	//solo quiero q entre la 1 vez q ve el agujero
		cont=0;
		ve_agujero=true;
		cout<<"VOY A VER AGUJERO"<<endl;
		donde_ve_agujero=veAgujero(sensores.terreno, current_state);
		cout<<"AGUJEROOO en "<<donde_ve_agujero<<endl;
		cogerAgujero(sensores.terreno, current_state, donde_ve_agujero);
		ngiros=0;
		accion=acciones[cont];
	}else if (ComprobarNVeces(current_state, nveces, nveces_aprox) && ngiros<3){
		//accion=giro_random();
		cout<<"comprueboNVeces"<<endl; 
		if (bien_situado)
			accion=giro_orientado(sensores.terreno, current_state, nveces); 
		else 
			accion=giro_orientado(sensores.terreno, current_state, nveces_aprox); 
		ngiros++;
		cout<<"he pasado por aqui ya mas de 4 veces"<<endl;
	}else if (sensores.terreno[2]!='P' and sensores.terreno[2]!='M' and ((sensores.terreno[1]=='M' and sensores.terreno[5]=='M') or (sensores.terreno[3]=='M' and sensores.terreno[7]=='M') or (sensores.terreno[1]=='P' and sensores.terreno[5]=='P') or (sensores.terreno[3]=='P' and sensores.terreno[7]=='P'))){
		accion=actFORWARD;
		cout<<"SIGO MURO"<<endl;
		ngiros=0;

		if (!zapatillas and sensores.terreno[2]=='B' and ngiros<4){
			accion=giro_random();
			ngiros++;
		}
		else if (!bikini and sensores.terreno[2]=='A' and ngiros<4){
			accion=giro_random_en_agua();
			ngiros++;
		}
		
	}else if((sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or sensores.terreno[2] == 'G') and sensores.superficie[2] == '_'){
		cout<<"estoy avanzando sin mas"<<endl;
		accion = actFORWARD;
		ngiros=0;
	}else if (sensores.terreno[2]=='K' or sensores.terreno[2]=='D' or sensores.terreno[2]=='X'){
		if (sensores.terreno[2]=='K'){
    		accion = actFORWARD;
			ngiros=0;
			cout<<"He cogido bikini"<<endl;
    	}else if (sensores.terreno[2]=='D'){
			accion = actFORWARD; 
			ngiros=0;
			cout<<"He cogido zapas"<<endl;
		}else if (sensores.terreno[2]=='X'){
			accion = actFORWARD;
			ngiros=0;
		}
	}else if (sensores.terreno[2]=='P' or sensores.terreno[2]=='M'){
		if (bien_situado)
			accion=giro_orientado(sensores.terreno, current_state, nveces); 
		else 
			accion=giro_orientado(sensores.terreno, current_state, nveces_aprox);
		if (accion==actFORWARD){	//no quiero que salga de la casilla
			accion=giro_random();
			cout<<"al final giro random"<<endl;
		}
		ngiros++;
	}else if (sensores.terreno[2] == 'B' and zapatillas==true){
		accion = actFORWARD;
		ngiros=0;	
	}else if (sensores.terreno[2] == 'A' and bikini==true){
		accion = actFORWARD;
		ngiros=0;	
	}else if ((sensores.terreno[2] == 'B' or sensores.terreno[2] == 'A') and ngiros<6){
		accion=giro_random();
		ngiros++;

		//if (sensores.terreno[2] == 'A')
		//	accion=giro_random_en_agua();

		/*int arena= mirar_terreno('T', sensores.terreno);
		cout<<"Veo arena en: "<<arena<<endl;
		int piedra= mirar_terreno('S', sensores.terreno);
		cout<<"Veo piedra en: "<<piedra<<endl;

		if (arena>0){
			accion=orientarse(arena);
			cout<<"voy a por arena"<<endl;
		}else if (piedra>0){
			accion=orientarse(piedra);
			cout<<"voy a por piedra"<<endl;
		}else 
			accion=actFORWARD;
		*/

	}else{
		cout<<"NO ME QUEDA OTRA Q AVANZAR"<<endl;
		accion = actFORWARD;
		/*int arena= mirar_terreno('T', sensores.terreno);
		int piedra= mirar_terreno('S', sensores.terreno);

		if (arena>0){
			accion=orientarse(mirar_terreno('T', sensores.terreno));
		}else if (piedra>0){
			accion=orientarse(mirar_terreno('S', sensores.terreno));
		}else 
			accion=actFORWARD;*/
		//ngiros--;	
		ngiros=0;
	}
	
	last_action = accion;
	ciclos--;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

