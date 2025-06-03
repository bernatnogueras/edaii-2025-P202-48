@startuml
!includeurl https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Component.puml

LAYOUT_TOP_DOWN()

' Sistema principal
System_Boundary(s1, "Cercador de Documents") {

  Container(webapp, "Interfície d'Usuari", "CLI", "Rep consultes i mostra resultats")

  Container(docModule, "Documents", "C Module", "Gestiona i deserialitza els documents")
  Container(linealModule, "Cerca Lineal", "C Module", "Fa cerques seqüencials segons consultes")
  Container(hashmapModule, "Index Invers (Hashmap)", "C Module", "Associa paraules amb documents")
  Container(graphModule, "Graph & Relevance", "C Module", "Calcula la rellevància i enllaços")

  Component(docDeserializer, "document_deserialize()", "Funció", "Deserialitza documents")
  Component(queryInit, "Query_init()", "Funció", "Inicialitza consultes")
  Component(searchLineal, "searchDocumentLineal()", "Funció", "Executa cerca lineal")
  Component(hashCreate, "HashMap_create()", "Funció", "Crea el mapa d'índex")
  Component(addIndex, "add_words_to_reverse_index()", "Funció", "Indexa paraules")
  Component(graphBuild, "crear_graph()", "Funció", "Construeix el graf")
  Component(scoreCalc, "relevance_score_filtered()", "Funció", "Calcula puntuació de rellevància")

  Rel(webapp, docModule, "Descarrega documents")
  Rel(webapp, linealModule, "Fa consultes")
  Rel(webapp, hashmapModule, "Fa cerques per paraules")
  Rel(webapp, graphModule, "Mostra documents rellevants")

  Rel(docModule, docDeserializer, "utilitza")
  Rel(linealModule, queryInit, "utilitza")
  Rel(linealModule, searchLineal, "utilitza")
  Rel(hashmapModule, hashCreate, "utilitza")
  Rel(hashmapModule, addIndex, "utilitza")
  Rel(graphModule, graphBuild, "utilitza")
  Rel(graphModule, scoreCalc, "utilitza")
}

@enduml


flowchart TD

    %% Nodo raíz
    DIAG["Diagrama"] 

    %% Rama Documents
    DIAG --> DOCUMENTS["Documents"]
    DOCUMENTS --> DOCUMENT["Document"]
    DOCUMENT --> FUNC_DESERIALIZE["document.deserialize()"]

    %% Rama Lineal
    DIAG --> LINEAL["Lineal"]
    LINEAL --> QNODE["QueryNode"]
    QNODE --> FUNC_Q_INIT["query.init()"]
    QNODE --> FUNC_SEARCH_DOC_LINEAL["search.document.lineal()"]

    %% Rama Hashmap
    DIAG --> HASHMAP["Hashmap"]
    HASHMAP --> DOCLIST["DocIdList"]
    DOCLIST --> FUNC_DOCLIST_CREATE["DocIdList.create()"]
    DOCLIST --> FUNC_DOCLIST_ADD["DocIdList.add()"]
    HASHMAP --> HASHNODE["HashNode"]
    HASHNODE --> FUNC_HASHMAP_INIT["HashMap.init()"]
    HASHNODE --> FUNC_ADD_WORD["add-word-to-reverse-index()"]

    %% Rama Graph
    DIAG --> GRAPH["Graph"]
    GRAPH --> GRAPH_NODE["Graph"]
    GRAPH_NODE --> FUNC_WAR_GRAPH["war-graph()"]
    GRAPH --> RELEVANCE["Relevance"]
    RELEVANCE --> FUNC_REL_SCAN["relevance-scan()"]
    RELEVANCE --> FUNC_FILTERED["filtered()"]

    %% Estilos de colores
    style DIAG fill:#eeedff,stroke:#333,stroke-width:1px

    style DOCUMENTS fill:#ccffcc,stroke:#333,stroke-width:1px
    style DOCUMENT fill:#ccffcc,stroke:#333,stroke-width:1px
    style FUNC_DESERIALIZE fill:#c8a2c8,stroke:#333,stroke-width:1px

    style LINEAL fill:#ccffcc,stroke:#333,stroke-width:1px
    style QNODE fill:#ccffcc,stroke:#333,stroke-width:1px
    style FUNC_Q_INIT fill:#c8a2c8,stroke:#333,stroke-width:1px
    style FUNC_SEARCH_DOC_LINEAL fill:#c8a2c8,stroke:#333,stroke-width:1px

    style HASHMAP fill:#ccffcc,stroke:#333,stroke-width:1px
    style DOCLIST fill:#ccffcc,stroke:#333,stroke-width:1px
    style FUNC_DOCLIST_CREATE fill:#c8a2c8,stroke:#333,stroke-width:1px
    style FUNC_DOCLIST_ADD fill:#c8a2c8,stroke:#333,stroke-width:1px
    style HASHNODE fill:#ccffcc,stroke:#333,stroke-width:1px
    style FUNC_HASHMAP_INIT fill:#c8a2c8,stroke:#333,stroke-width:1px
    style FUNC_ADD_WORD fill:#c8a2c8,stroke:#333,stroke-width:1px

    style GRAPH fill:#ccffcc,stroke:#333,stroke-width:1px
    style GRAPH_NODE fill:#ccffcc,stroke:#333,stroke-width:1px
    style FUNC_WAR_GRAPH fill:#c8a2c8,stroke:#333,stroke-width:1px
    style RELEVANCE fill:#ccffcc,stroke:#333,stroke-width:1px
    style FUNC_REL_SCAN fill:#c8a2c8,stroke:#333,stroke-width:1px
    style FUNC_FILTERED fill:#c8a2c8,stroke:#333,stroke-width:1px

    %% Leyenda (opcional)
    subgraph Leyenda["Leyenda"]
        green["Componente/ms en memoria"] 
        purple["Funciones/operaciones"]
    end

    style green fill:#ccffcc,stroke:#333,stroke-width:1px
    style purple fill:#c8a2c8,stroke:#333,stroke-width:1px
