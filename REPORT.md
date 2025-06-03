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