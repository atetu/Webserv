#include <config/block/LocationBlock.hpp>
#include <config/block/RootBlock.hpp>
#include <config/block/ServerBlock.hpp>
#include <exception/IOException.hpp>
#include <http/HTTPClient.hpp>
#include <http/HTTPHeaderFields.hpp>
#include <http/HTTPMethod.hpp>
#include <http/HTTPOrchestrator.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPRequestParser.hpp>
#include <http/HTTPValueParser.hpp>
#include <http/HTTPResponse.hpp>
#include <http/HTTPStatus.hpp>
#include <http/HTTPVersion.hpp>
#include <encoding/default/chunk/ChunkDecoder.hpp>
#include <encoding/default/chunk/ChunkEncoder.hpp>
//#include <io/SocketServer.hpp>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/time.h>
//#include <util/buffer/BaseBuffer.hpp>
//#include <util/buffer/IOBuffer.hpp>
#include <util/Enum.hpp>
#include <util/log/Logger.hpp>
#include <util/log/LoggerFactory.hpp>
#include <util/System.hpp>
#include <util/URL.hpp>
#include <set>
#include <utility>
#include <string>


#include <http/handler/HTTPMethodHandler.hpp>
#include <http/handler/methods/GetHandler.hpp>
#include <http/HTTPRequest.hpp>
#include <http/HTTPHeaderParser.hpp>

int main()


{
	std::string body = "4\r\n"
		"Vo\r\n"
		"1C\r\n"
		"et voici un second morceau\r\n"
		"20\r\n"
		"et voici deux derniers morceaux \r\n"
		"12\r\n"
		"sans saut de ligne\r\n"
		"0\r\n"
		"\r\n";
	
	ChunkDecoder decoder(body);
	std::cout << decoder.decode() << std::endl;

	std::string body2 = "sans métaphore mais au moins qu'il ait quelque tenue que mon visage sans retenue puisse soudain passer pour un tissu une pierre un songe soit en quelque sorte un tableau fasse tableau mais ce n'est pas le cas même ce mot albâtre jeté au visage jeté tout à trac sur la page en haut de page ce mot me défigure ne me figure pas ne me représente pas ne figure rien de ce que je suis de ce que je pense être et je suis encore et toujours circonspect dans le doute et ce mot n'apporte rien aucune réponse et donc toujours je me jette à la figure ces accusations comme des bouteilles non pas à la mer mais bien dans la gueule oui je me donne des coups de bouteille tessons épars sur le parquet et mes joues ensanglantées enfin que ce soit ou non métaphore que le mot d'albâtre me figure ou non je prends ces coups ces reproches en plein visage et je m'accuse d'être velléitaire aussi bien sûr pour trop entreprendre je lance cent feux il est normal qu'un certain nombre des foyers meure et même ne démarre qu'à peine avant de s'achever dans un bruit de feuilles mouillées de bois mort de bois trop vert encore pour prendre tout cela encore métaphore et toujours métaphore peut-être est-ce le mot albâtre qui appelle autant de métaphores ou bien les conditions d'écriture du très long texte que par facétie ou encore autodérision je pourrais être tenté de rebaptiser très long texte interrompu et l'adjectif interrompu ici au milieu de la ligne interrompt mes songes interrompt le torrent de sornettes lance d'autres tirades propose peut-être d'autres charades mais pour mieux me ramener vers le rivage bourbeux où je ne cesse de me lancer ces reproches à la figure velléitaire velléitaire et me voici encore à ne pas même essayer de me justifier moi-même de tout cela feux mal éteints et feux qui n'ont jamais pris aussi me trouvé-je vingt vaines justification";

	ChunkEncoder encoder(body2);
	std::cout << encoder.encode() << std::endl;

}