package aed;
public class SistemaCNE {
    private Distrito[] distritosArray;
    private String[] partidosString;
    private PresidenteResultados presidenteArray;
    private Heap[] dhondtCocientes;
    private int [][] resDiputados;

    public class VotosPartido{
        private int presidente;
        private int diputados;
        VotosPartido(int presidente, int diputados){this.presidente = presidente; this.diputados = diputados;}
        public int votosPresidente(){return presidente;}
        public int votosDiputados(){return diputados;}
    }

    /**
     * Inicializa el sistema de la Cámara Nacional Electoral. O(P ∗ D)
     */
    public SistemaCNE(String[] nombresDistritos, int[] diputadosPorDistrito, String[] nombresPartidos, int[] ultimasMesasDistritos) {
        presidenteArray = new PresidenteResultados(nombresPartidos.length); // O(1)
        distritosArray = new Distrito[nombresDistritos.length]; // O(1)
        dhondtCocientes = new Heap[nombresDistritos.length];// O(1)
        resDiputados = new int[nombresDistritos.length][nombresPartidos.length - 1]; // O(1),  saco votos en blanco, inicia todo en null

        //iniciliza una nueva instancia de clase Distrito para cada distrito del sistema O(D)
        for (int i = 0 ; i < nombresDistritos.length ; i++) {
            Distrito nuevoDistrito = new Distrito(
                    nombresPartidos.length,
                    nombresDistritos[i],
                    diputadosPorDistrito[i],
                    ultimasMesasDistritos[i]
            ); // O(1)

            distritosArray[i] = nuevoDistrito; // O(1)

        }
        partidosString = nombresPartidos; // O(1)
    }

    /**
     * Devuelve el nombre del partido a partir de su id. O(1)
     */
    public String nombrePartido(int idPartido) {
        return partidosString[idPartido];
    }

    /**
     * Devuelve el nombre del distrito a partir de su id. O(1)
     */
    public String nombreDistrito(int idDistrito) {
        return distritosArray[idDistrito].nombre;
    }

    /**
     * Devuelve la cantidad de bancas de diputados en disputa en un distrito dado. O(1)
     */
    public int diputadosEnDisputa(int idDistrito) {
        return distritosArray[idDistrito].cantDeBancas;
    }

    /**
     * Devuelve el nombre del distrito al que pertenece una mesa. O(log(D))
     */
    public String distritoDeMesa(int idMesa) {
        int index = busquedaBinariaDistrictos(distritosArray, idMesa); // O(log(D))

        return distritosArray[index].nombre;
    }

    /**
     * Realiza búsqueda binaria en el array ingresado, en nuestro caso de un array de distritos O(Log(D))
     */
    private int busquedaBinariaDistrictos(Distrito[] array, int valorBuscado) {
        int indexComienzo = 0;
        int indexFinal = array.length - 1;
        int indexMedio = (indexFinal - indexComienzo) / 2;

        while (indexFinal - indexComienzo >= 2) {
            if (array[indexMedio].ultimaMesa > valorBuscado) {
                indexFinal = indexMedio;
            } else if (array[indexMedio].ultimaMesa < valorBuscado) {
                indexComienzo = indexMedio;
            } else {
                return indexMedio + 1;
            }
            indexMedio = indexComienzo + (indexFinal - indexComienzo) / 2;
        }
    
        if (array[indexComienzo].ultimaMesa > valorBuscado) {
            return indexComienzo;
        } else {
            return indexComienzo + 1; //cuando hay solo 2 elem final = comienzo + 1
                                      // quiero el failsafe por si se da el caso de que final = comienzo < valorBuscado
        }
    
    }


    /**
     * Registra los votos de una mesa. O(P + log(D))
     */
    public void registrarMesa(int idMesa, VotosPartido[] actaMesa) {
        //usa busqueda binaria log(n) para encontrar el índice del distrito
        int idDistrito = busquedaBinariaDistrictos(distritosArray, idMesa); // O(log(D))
        Distrito distrito = distritosArray[idDistrito];

        cargarMesaAlSistema(distrito, actaMesa, idDistrito); // O(P)

        int indice = presidenteArray.votos.length - 1;
        actualizarHeapDhondt(indice, idDistrito); // O(P)

        resDiputados[idDistrito] = null;

        actualizarPosPresidencial(); // O(P)

        // Complejidad final: O(log(D)) + O(P) + O(P) + O(P) = O(P + log(D))
    }


    public void cargarMesaAlSistema(Distrito distrito, VotosPartido[] actaMesa, int idDistrito) {
        for (int i = 0 ; i < actaMesa.length ; i++) { // O(P) porque carga los votos a todos los partidos
            distrito.votosDiputados[i] += actaMesa[i].votosDiputados();
            distrito.votosDiputadosTotal += actaMesa[i].votosDiputados();
            presidenteArray.votos[i] += actaMesa[i].votosPresidente();
            presidenteArray.sumaTodosLosVotos += actaMesa[i].votosPresidente();
        }
        // Complejidad final: O(P) + O(P - 1) = O(P)

    }

    public void actualizarHeapDhondt(int indice, int idDistrito){
        Partido[] partidosActualizado = new Partido[indice];

        for (int j = 0; j < indice; j++) { // inidice es presidenteArray.votos.length - 1 => O(P - 1)
            double umbral = ((double) (distritosArray[idDistrito].votosDiputados[j]) /( distritosArray[idDistrito].votosDiputadosTotal)); // O(1)

            if (umbral > 0.030) {
                partidosActualizado[j] = new Partido(j, distritosArray[idDistrito].votosDiputados[j]); // O(1)

            } else {
                // los que no superen el 3% le asigno 0 votos al heap así cuando calculamos las bancas no interfieren
                partidosActualizado[j] = new Partido(j, 0); // O(1)
            }
        }

        Heap heapActualizado = new Heap(indice, partidosActualizado); // ordena partidosActualizados con algoritmo de floyd => O(P - 1)

        dhondtCocientes[idDistrito] = heapActualizado; // O(1)

        // Complejidad final: O(P - 1) + O(P - 1) + O(1) = O(P - 1) =~ O(P)
    }
    public void actualizarPosPresidencial() {
        int[] votos = presidenteArray.votos; // O(1)

        for (int i = 0 ; i < presidenteArray.votos.length - 1; i ++) { // saco votos en blanco
                                                                        // O(P -1)
            if (presidenteArray.primeroId == -1 || votos[i] >= votos[presidenteArray.primeroId]) { // O(1)
                presidenteArray.primeroId = i; // O(1)
            } else if (presidenteArray.segundoId == -1 || votos[i] >= votos[presidenteArray.segundoId]) { // O(1)
                presidenteArray.segundoId = i; // O(1)
            }
        }
        // Complejidad final: O(P - 1) =~ O(P)
    }

    /**
     * Devuelve la cantidad de votos que obtuvo un partido sobre su candidata/o a presidente a nivel
     * nacional. O(1)
     */
    public int votosPresidenciales(int idPartido) {
        return presidenteArray.votos[idPartido];
    }

    /**
     * Devuelve la cantidad de votos que obtuvo un partido en sus diputados en un distrito dado. O(1)
     */
    public int votosDiputados(int idPartido, int idDistrito) {
        Distrito distrito = distritosArray[idDistrito];
        return distrito.votosDiputados[idPartido];
    }

    /**
     *  Devuelve la cantidad de bancas que obtuvo cada partido en un distrito dado d. O(Dd ∗ log(P))
     */
    public int[] resultadosDiputados(int idDistrito){
        int[] resultados = new int[partidosString.length - 1];

        // si se cargaron votos y no lo calculé desde la última carga de votos en ese distrito,
        // veo cuántas bancas le corresponde a c/partido en ese distrito
        if (resDiputados[idDistrito] == null && distritosArray[idDistrito].votosDiputadosTotal != 0) {

            Distrito distritoActual = distritosArray[idDistrito]; // O(1)
            Heap heapActual = dhondtCocientes[idDistrito]; // O(1)
            int cantBancasDistrito = distritoActual.cantDeBancas; // O(1)

            for (int i = 0 ; i < cantBancasDistrito ; i++) { // O(Dd)
                Partido actualMaximo = heapActual.maximo(); // O(1)

                int nuevoCociente = actualMaximo.votosDiputadosDistrito / (actualMaximo.divisiones + 1); // O(1)
                heapActual.actualizarMaximo(nuevoCociente); // O(log(P))

                resultados[actualMaximo.id] += 1; // O(1)
            }

            resDiputados[idDistrito] = resultados; // O(1)

        } else if (resDiputados[idDistrito] != null) { // O(1)
            // si no se cargaron votos en ese distrito desde la última carga, devuelvo lo cargado anteriormente
            resultados = resDiputados[idDistrito];  // O(1)

        }

        return resultados; // O(1)

        // Complejidad final: O(Dd * O(log(P))
    }

    /**
     * Devuelve true si hay ballotage, false en caso contrario. O(1)
     */
    public boolean hayBallotage() {
        int votosTotales = presidenteArray.sumaTodosLosVotos;  // O(1)
        int votosPrimero = presidenteArray.votos[presidenteArray.primeroId];  // O(1)
        int votosSegundo = presidenteArray.votos[presidenteArray.segundoId];  // O(1)
        boolean res = true;  // O(1)

        if (supera45(votosPrimero, votosTotales)) { // O(1)
            res = false; // O(1)
        } else if (supera40ConDifDe10(votosPrimero, votosSegundo, votosTotales)) { // O(1)
            res = false; // O(1)
        }

        return res; // O(1)
    }
    public boolean supera45 (int votosPrimero, int votosTotales){
        return ((double) votosPrimero / votosTotales) > 0.45;  // O(1)
    }

    public boolean supera40ConDifDe10 (int votosPrimero, int votosSegundo, int votosTotales){
        boolean  superaCuarenta = ((double) votosPrimero/ votosTotales >= 0.40); // O(1)
        boolean diferenciaDiez = ((double) (votosPrimero - votosSegundo) / votosTotales) >= 0.10; // O(1)
        return superaCuarenta && diferenciaDiez; // O(1)
    }

}

