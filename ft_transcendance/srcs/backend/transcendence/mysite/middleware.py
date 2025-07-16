import os
from django.http import JsonResponse
from dotenv import load_dotenv

load_dotenv()  # Charger les variables d'environnement

class BlockDirectAPIAccessMiddleware:
    def __init__(self, get_response):
        self.get_response = get_response

        # Charger les hostnames autorisés depuis .env
        allowed_hosts = os.getenv("ALLOWED_HOSTNAME", "localhost")
        self.allowed_hosts = [host.strip() for host in allowed_hosts.split(",")]

    def __call__(self, request):
        if request.path.startswith("/api/") and not request.path.startswith("/api/call"):
            referer = request.META.get("HTTP_REFERER", "")
            host = request.META.get("HTTP_HOST", "")

            # Vérifier si l'origine est dans la liste des hôtes autorisés
            if any(allowed in referer for allowed in self.allowed_hosts):
                return self.get_response(request)

            return JsonResponse({"error": "Accès interdit"}, status=403)

        return self.get_response(request)
