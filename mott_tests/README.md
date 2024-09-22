
Einleitung

Das **Hypertext Transfer Protocol (HTTP)** ist ein Anwendungsprotokoll, das für verteilte, kollaborative und hypermediale Informationssysteme verwendet wird.

Es bildet die Grundlage für die Datenkommunikation im **World Wide Web**, bei dem Hypertext-Dokumente Verweise (Hyperlinks) auf andere Ressourcen enthalten, die der Nutzer leicht durch Anklicken oder Tippen im Webbrowser aufrufen kann.

**HTTP** wurde speziell entwickelt, um die Nutzung von Hypertext und die Entwicklung des World Wide Web zu unterstützen.

Eine der Hauptaufgaben eines Webservers besteht darin, Webinhalte (z. B. Webseiten) zu speichern, zu verarbeiten und an die Clients auszuliefern. Die Kommunikation zwischen Client (oft ein Webbrowser) und Server erfolgt über das Hypertext Transfer Protocol. Die ausgelieferten Seiten sind dabei meist **HTML-Dokumente**, die zusätzlich zu Text auch Bilder, Stylesheets und Skripte enthalten können.

Für stark frequentierte Websites können **mehrere Webserver** eingesetzt werden, um die Last der Anfragen zu verteilen und die Performance zu verbessern.

Ein **User-Agent** – oft ein Webbrowser oder ein Webcrawler (z.B. für Suchmaschinen) – startet die Kommunikation, indem er über HTTP eine bestimmte Ressource (z. B. eine Webseite) vom Server anfordert. Der Server antwortet entweder mit dem angeforderten Inhalt oder mit einer Fehlermeldung, wenn die Ressource nicht bereitgestellt werden kann. Diese angeforderte Ressource ist häufig eine reale Datei, die auf dem Speicher des Servers abgelegt ist. Es kann jedoch auch eine dynamisch erzeugte Antwort sein, abhängig davon, wie der Webserver implementiert ist.

Obwohl die primäre Aufgabe des Servers darin besteht, Inhalte zu liefern, umfasst HTTP auch Mechanismen zur **Entgegennahme von Inhalten** seitens der Clients. Dies wird beispielsweise beim Absenden von Webformularen oder dem Hochladen von Dateien verwendet. Diese Funktion ist eine wichtige Erweiterung des Protokolls, da sie die Interaktion zwischen Nutzer und Website ermöglicht und dynamische Inhalte unterstützt.

Zusammengefasst bietet HTTP die **technische Grundlage**, um die Funktionalität des Internets, wie wir es heute kennen, zu ermöglichen. Es ist entscheidend für den Austausch von Informationen zwischen **Client** und **Server** und bildet die Basis für eine Vielzahl von Online-Interaktionen – von einfachen Webseiten bis hin zu komplexen Webanwendungen.



telnet, curl, netcat
Telnet was once used to connect directly to web servers, mail servers, and more for testing. Today, more specialized tools like curl and netcat are commonly used for testing specific network services.
bash: telnet <hostname> <port>


TCP = Transmission Control Protocol
