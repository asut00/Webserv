/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:53:52 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/19 14:53:53 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define HTTP_CONTINUE						100  // Requête reçue, en attente de suite
#define HTTP_SWITCHING_PROTOCOLS			101  // Serveur accepte de changer de protocole

#define HTTP_OK								200  // Requête réussie
#define HTTP_CREATED						201  // Requête réussie et ressource créée
#define HTTP_ACCEPTED						202  // Requête acceptée mais non traitée immédiatement
#define HTTP_NON_AUTHORITATIVE_INFORMATION	203  // Informations retournées, mais provenant d'une source non autoritaire
#define HTTP_NO_CONTENT						204  // Requête réussie mais pas de contenu à renvoyer
#define HTTP_RESET_CONTENT					205  // Requête réussie, demande au client de réinitialiser l'affichage
#define HTTP_PARTIAL_CONTENT				206  // Requête réussie, mais renvoie une partie seulement du contenu

#define HTTP_MULTIPLE_CHOICES				300  // Plusieurs options possibles
#define HTTP_MOVED_PERMANENTLY				301  // Ressource déplacée de manière permanente
#define HTTP_FOUND							302  // Ressource temporairement déplacée (souvent utilisé pour des redirections)
#define HTTP_SEE_OTHER						303  // Voir autre URI pour la réponse
#define HTTP_NOT_MODIFIED					304  // La ressource n'a pas été modifiée depuis la dernière requête
#define HTTP_USE_PROXY						305  // La ressource doit être accédée via un proxy
#define HTTP_TEMPORARY_REDIRECT				307  // Redirection temporaire, mais garder la méthode HTTP
#define HTTP_PERMANENT_REDIRECT				308  // Redirection permanente, méthode HTTP à conserver

#define HTTP_BAD_REQUEST                     400  // Requête mal formulée
#define HTTP_UNAUTHORIZED                    401  // Authentification requise
#define HTTP_PAYMENT_REQUIRED                402  // Paiement requis (non utilisé généralement)
#define HTTP_FORBIDDEN                       403  // Accès refusé ou interdit
#define HTTP_NOT_FOUND                       404  // Ressource non trouvée
#define HTTP_METHOD_NOT_ALLOWED              405  // Méthode HTTP non autorisée pour la ressource
#define HTTP_NOT_ACCEPTABLE                  406  // Le serveur ne peut pas générer une réponse acceptable pour le client
#define HTTP_PROXY_AUTHENTICATION_REQUIRED   407  // Authentification proxy requise
#define HTTP_REQUEST_TIMEOUT                 408  // Temps de requête écoulé
#define HTTP_CONFLICT                        409  // Conflit lors du traitement de la requête
#define HTTP_GONE                            410  // Ressource supprimée et non disponible
#define HTTP_LENGTH_REQUIRED                 411  // En-tête Content-Length requis
#define HTTP_PRECONDITION_FAILED             412  // Précondition de la requête échouée
#define HTTP_PAYLOAD_TOO_LARGE               413  // Charge utile trop volumineuse
#define HTTP_URI_TOO_LONG                    414  // URI trop longue pour être traitée par le serveur
#define HTTP_UNSUPPORTED_MEDIA_TYPE          415  // Format de media non supporté par le serveur
#define HTTP_RANGE_NOT_SATISFIABLE           416  // Les valeurs de la plage demandée ne peuvent pas être satisfaites
#define HTTP_EXPECTATION_FAILED              417  // Expectation échouée (entête Expect)
#define HTTP_IM_A_TEAPOT                     418  // I'm a teapot (RFC 2324, April Fool's joke)
#define HTTP_MISDIRECTED_REQUEST             421  // Requête redirigée vers un serveur inapproprié
#define HTTP_UNPROCESSABLE_ENTITY            422  // La requête est bien formée mais ne peut pas être traitée
#define HTTP_LOCKED                          423  // La ressource est verrouillée
#define HTTP_FAILED_DEPENDENCY               424  // Dépendance échouée
#define HTTP_TOO_EARLY                       425  // Requête envoyée trop tôt
#define HTTP_UPGRADE_REQUIRED                426  // Le client doit passer à un autre protocole
#define HTTP_PRECONDITION_REQUIRED           428  // La requête doit avoir des préconditions
#define HTTP_TOO_MANY_REQUESTS               429  // Trop de requêtes envoyées dans un court laps de temps
#define HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE 431  // Les en-têtes de requête sont trop volumineux
#define HTTP_UNAVAILABLE_FOR_LEGAL_REASONS   451  // Ressource indisponible pour des raisons légales

#define HTTP_INTERNAL_SERVER_ERROR           500  // Erreur interne du serveur
#define HTTP_NOT_IMPLEMENTED                 501  // Fonctionnalité non implémentée
#define HTTP_BAD_GATEWAY                     502  // Mauvaise réponse reçue d'un autre serveur
#define HTTP_SERVICE_UNAVAILABLE             503  // Le service est temporairement indisponible
#define HTTP_GATEWAY_TIMEOUT                 504  // Temps de réponse du serveur en aval écoulé
#define HTTP_VERSION_NOT_SUPPORTED           505  // Version HTTP non supportée
#define HTTP_VARIANT_ALSO_NEGOTIATES         506  // Problème de négociation de contenu
#define HTTP_INSUFFICIENT_STORAGE            507  // Espace insuffisant pour traiter la requête
#define HTTP_LOOP_DETECTED                   508  // Boucle détectée dans le traitement de la requête
#define HTTP_NOT_EXTENDED                    510  // Requêtes supplémentaires nécessaires
#define HTTP_NETWORK_AUTHENTICATION_REQUIRED 511  // Authentification réseau requise

