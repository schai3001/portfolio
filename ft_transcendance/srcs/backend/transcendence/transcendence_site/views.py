from django.shortcuts import render, redirect
from django.contrib import messages
from django.contrib.auth import update_session_auth_hash
from django.conf import settings
from .forms import UserRegisterForm
from rest_framework import generics
from .models import MyUser, UserStats, GameStats
from .serializers import UserSerializer, nicknameSerializer, registerSerializer, loginSerializer, friendSerializer, otpSerializer, statsSerializer, tournamentSerializer, FriendsSerializer, OauthUserSerializer, PasswordModifSerializer, emailSerializer, TournamentEndSerializer
from rest_framework import status
from rest_framework_simplejwt.views import TokenObtainPairView, TokenRefreshView
from rest_framework_simplejwt.tokens import AccessToken, RefreshToken
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from django.core.validators import validate_email #importer la fonction validate_email
from django.core.exceptions import ValidationError #importer la classe ValidationError
from django.contrib.auth import authenticate, login, logout #importer les fonctions authenticate et login
from django.views.decorators.csrf import csrf_exempt, ensure_csrf_cookie #importer les décorateurs csrf_exempt et ensure_csrf_cookie
from django.middleware.csrf import get_token #importer la fonction get_token
from django.http import HttpResponse, JsonResponse #importer les classes HttpResponse et JsonResponse
import random #importer le module random
from .utils import generate_otp_code, send_otp_email #importer les fonctions generate_otp et send_otp_email
import requests, os, re, random, string  #importer les modules
from rest_framework.views import APIView
from rest_framework.parsers import MultiPartParser, FormParser



# Create your views here.

def transcendence_site(request):
    return render(request, 'transcendence_site.html', {})

class UserListCreateView(generics.ListCreateAPIView):
    queryset = MyUser.objects.all()
    serializer_class = UserSerializer

class UserDetailView(generics.RetrieveUpdateDestroyAPIView):
    queryset = MyUser.objects.all()
    serializer_class = UserSerializer

# Vues d'authentification pour le token JWT
class CustomTokenObtainPairView(TokenObtainPairView):
    # Personnaliser l'obtention du token si nécessaire
    pass

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def get_user(request):
    user = request.user
    return Response({
        "username": user.username, 
        "email": user.email,
        "nickname": user.nickname if hasattr(user, 'nickname') else user.username
    })

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def getuserfriendlist_view(request):
     user = request.user
     friends = user.friends.all()
     serializer = FriendsSerializer(friends, many=True)
     return Response(serializer.data)

# @api_view(['POST'])
# @permission_classes([IsAuthenticated])
# def add_friend(request, friend_id):
#     user = request.user
#     try:
#         friend = MyUser.objects.get(id=friend_id)
#         if friend == user:
#             return Response({"error" : "Vous ne pouvez pas vous ajouter vous-même"}, status=status.HTTP_400_BAD_REQUEST)
        
#         user.friends.add(friend)
#         return Response({"message": "Ami ajouté avec succès."}, status=status.HTTP_200_OK)
    
#     except MyUser.DoesNotExist:
#         return Response({"error": "Utilisateur introuvable."}, status=status.HTTP_404_NOT_FOUND)
    

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def remove_friend(request, friend_id):
    user = request.user
    try:
        friend = MyUser.objects.get(id=friend_id)
        user.friends.remove(friend)
        return Response({"message": "Ami supprimé avec succès."}, status=status.HTTP_200_OK)

    except MyUser.DoesNotExist:
        return Response({"error": "Utilisateur introuvable."}, status=status.HTTP_404_NOT_FOUND)




#endpoint a gérer_________________________________________________________________________________________



#Serializer : clean way to validate input data
# - Required fields are present
# - Data is in the correct format
# - Custom validation rules are applied
# - easy data transformation


@api_view(['POST']) #décorateur pour la méthode POST : restrict the view to accept only POST requests : only data submision no data retrieval
@permission_classes([IsAuthenticated]) #décorateur pour vérifier si l'utilisateur est authentifié
def modif_nickname(request):    #modifier le nickname
    try:
        serializer = nicknameSerializer(data=request.data)  #créer une instance de nicknameSerializer avec les données de la requête
        if serializer.is_valid(): #vérifier si les données sont valides
            data = serializer.validated_data #récupérer les données validées dans data
            nickname = data['nickname'] #récupérer le nickname
            request.user.nickname = nickname #modifier le nickname de l'utilisateur
            request.user.save() #sauvegarder les modifications
            return Response({"message": "Nickname modifié avec succès."}, status=status.HTTP_200_OK) #retourner un message de succès
        else:
            return Response({"error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST) #retourner les erreurs de validation
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR) #retourner l'erreur catch


@api_view(['POST'])
@permission_classes([IsAuthenticated])
def change_email(request):
    try:
        serializer = emailSerializer(data=request.data)
        if serializer.is_valid():
            data = serializer.validated_data
            try:
                validate_email(data['email']) #vérifier le format de l'email
            except ValidationError :
                return Response({"error": "Format d'email invalide."}, status=status.HTTP_400_BAD_REQUEST)
            if (MyUser.objects.filter(email=data['email']).exists()): #vérifier si l'email existe déjà 
                return Response({"error": "Cet email existe déjà."}, status=status.HTTP_400_BAD_REQUEST)
            email = data['email']
            request.user.email = email
            request.user.save()
            return Response({"message": "Email modifié avec succès."}, status=status.HTTP_200_OK) #retourner un message de succès
        else:
            return Response({"error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST) #retourner les erreurs de validation
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR) #retourner l'erreur catch





@api_view(['POST']) 
def register_view(request): 
    try:
        serializer = registerSerializer(data=request.data)  #créer une instance de registerSerializer avec les données de la requête
        if serializer.is_valid(): #vérifier si les données sont valides
            data = serializer.validated_data #récupérer les données validées dans data
            username = data['username'] #récupérer le username
            email = data['email'] #récupérer l'email
            password1 = data['password1'] #récupérer le password1
            password2 = data['password2'] #récupérer le password2


            try:
                validate_email(email) #vérifier le format de l'email
            except ValidationError :
                return Response({"error": "Format d'email invalide."}, status=status.HTTP_400_BAD_REQUEST)
            
            if password1 != password2: #vérifier si les mots de passe correspondent
                return Response({"error": "Les mots de passe ne correspondent pas."}, status=status.HTTP_400_BAD_REQUEST)
            
            if len(password1) < 8: #vérifier si le mot de passe contient au moins 8 caractères
                return Response({"error": "Le mot de passe doit contenir au moins 8 caractères."}, status=status.HTTP_400_BAD_REQUEST)

            if not re.findall('[A-Z]', password1): #vérifier si le mot de passe contient au moins une lettre majuscule
                return Response({"error": "Le mot de passe doit contenir au moins une lettre majuscule."}, status=status.HTTP_400_BAD_REQUEST)

            if not re.findall('[a-z]', password1): #vérifier si le mot de passe contient au moins une lettre minuscule
                return Response({"error": "Le mot de passe doit contenir au moins une lettre minuscule."}, status=status.HTTP_400_BAD_REQUEST)

            if not re.findall('[0-9]', password1): #vérifier si le mot de passe contient au moins un chiffre
                return Response({"error": "Le mot de passe doit contenir au moins un chiffre."}, status=status.HTTP_400_BAD_REQUEST)
            
            if not re.findall(r'\W', password1): #vérifier si le mot de passe contient au moins un caractère spécial
                return Response({"error": "Le mot de passe doit contenir au moins un caractère spécial."}, status=status.HTTP_400_BAD_REQUEST)

            if (MyUser.objects.filter(username=username).exists()): #vérifier si le username existe déjà
                return Response({"error": "Ce nom d'utilisateur existe déjà."}, status=status.HTTP_400_BAD_REQUEST)

            if (MyUser.objects.filter(email=email).exists()): #vérifier si l'email existe déjà 
                return Response({"error": "Cet email existe déjà."}, status=status.HTTP_400_BAD_REQUEST)

            user = MyUser.objects.create_user(username=username, email=email, password=password1, nickname=username) #créer un utilisateur
            user.stat = UserStats.objects.create(user=user) #créer les statistiques de l'utilisateur
            return Response({"message": "Utilisateur créé avec succès."}, status=status.HTTP_201_CREATED)
        else:
            return Response({"error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST) #retourner les erreurs de validation
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


@api_view(['POST'])
@permission_classes([IsAuthenticated])
def change_password(request):
     try:
          serializer = PasswordModifSerializer(data = request.data)
          if serializer.is_valid():
                data = serializer.validated_data
                currpass = data['current_password']
                newpass = data['new_password']
                user = request.user
                if not currpass or not newpass:
                    return JsonResponse({"error": "Tous les champs sont obligatoires."}, status=status.HTTP_400_BAD_REQUEST)
                if not user.check_password(currpass):
                    return JsonResponse({"error": "Ancien mot de passe incorrect."}, status=status.HTTP_400_BAD_REQUEST)

                if len(newpass) < 8: #vérifier si le mot de passe contient au moins 8 caractères
                    return Response({"error": "Le mot de passe doit contenir au moins 8 caractères."}, status=status.HTTP_400_BAD_REQUEST)
    
                if not re.findall('[A-Z]', newpass): #vérifier si le mot de passe contient au moins une lettre majuscule
                    return Response({"error": "Le mot de passe doit contenir au moins une lettre majuscule."}, status=status.HTTP_400_BAD_REQUEST)
    
                if not re.findall('[a-z]', newpass): #vérifier si le mot de passe contient au moins une lettre minuscule
                    return Response({"error": "Le mot de passe doit contenir au moins une lettre minuscule."}, status=status.HTTP_400_BAD_REQUEST)
    
                if not re.findall('[0-9]', newpass): #vérifier si le mot de passe contient au moins un chiffre
                    return Response({"error": "Le mot de passe doit contenir au moins un chiffre."}, status=status.HTTP_400_BAD_REQUEST)
                
                if not re.findall(r'\W', newpass): #vérifier si le mot de passe contient au moins un caractère spécial
                    return Response({"error": "Le mot de passe doit contenir au moins un caractère spécial."}, status=status.HTTP_400_BAD_REQUEST)
                user.set_password(newpass)
                user.save()
                update_session_auth_hash(request, user)
                return JsonResponse({"message": "password modifié avec succès !"}, status=status.HTTP_200_OK)
          else:
               return JsonResponse({"error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
     except Exception as e:
          return JsonResponse({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

                

                

@api_view(['POST'])
def login_view(request):
    try:
        serializer = loginSerializer(data=request.data) #créer une instance de loginSerializer avec les données de la requête
        if serializer.is_valid(): #vérifier si les données sont valides
            data = serializer.validated_data #récupérer les données validées dans data
            username = data['username'] #récupérer le username
            password = data['password'] #récupérer le password
            user = authenticate(username=username, password=password) #builtin authentifier l'utilisateur
            if user is not None: #vérifier si l'utilisateur existe

                if not user.check_2fa: #vérifier si l'utilisateur a activé l'authentification à deux facteurs

                    login(request, user) #builtin connecter l'utilisateur
                    user.check_online = True #mettre l'utilisateur en ligne
                    user.save() #sauvegarder les modifications
                    token = AccessToken.for_user(user) #générer un token
                    encoded_token = str(token) #encoder le token
                    refresh = RefreshToken.for_user(user)
                    user_data = { #créer un dictionnaire avec les données de l'utilisateur
                        'message': 'Connexion réussie.', #message de succès
					    'username' : getattr(user, 'username', 'unknown'), #récupérer le username de l'utilisateur
					    'nickname' : getattr(user, 'nickname', 'unknown'), #récupérer le nickname de l'utilisateur
					    'email' : getattr(user, 'email', 'unknown'), #récupérer l'email de l'utilisateur
					    'access': encoded_token, #récupérer le token
                        'refresh': str(refresh),
                    }
                    return Response(user_data, status=status.HTTP_200_OK) #retourner les données de l'utilisateur

                else:
                    otp = generate_otp_code() #générer un code otp
                    user.otp_code = otp #enregistrer le code otp
                    user.save() #sauvegarder les modifications
                    send_otp_email(user.email, otp) #envoyer le code otp par email
                    return Response({"message": "Veuillez entrer le code OTP pour vous connecter.", 'username': username, 'otp': True}, status=status.HTTP_202_ACCEPTED)
            else:
                return Response({"error": "Nom d'utilisateur ou mot de passe incorrect."}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"error": serializer.errors}, status=401) #retourner les erreurs de validation
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

@permission_classes([IsAuthenticated])
def logout_view(request):
    try:
        if request.method == 'POST': #vérifier si la méthode est POST
            if request.user.is_authenticated: #vérifier si l'utilisateur est authentifié
                request.user.check_online = False #mettre l'utilisateur hors ligne
                request.user.save() #sauvegarder les modifications
                logout(request) #builtin déconnecter l'utilisateur
            return JsonResponse({"message": "Déconnexion réussie."}, status=status.HTTP_200_OK) #retourner un message de succès
        else:
            return JsonResponse({"error": "Méthode non autorisée."}, status=status.HTTP_405_METHOD_NOT_ALLOWED) #retourner une erreur         
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

@api_view(['POST'])
@permission_classes([IsAuthenticated]) #check si ca fait crash
def logwithoutlogin(request):
    try:
        serializer = loginSerializer(data=request.data) #créer une instance de loginSerializer avec les données de la requête
        if serializer.is_valid(): #vérifier si les données sont valides
            data = serializer.validated_data #récupérer les données validées dans data
            username = data['username'] #récupérer le username
            password = data['password'] #récupérer le password
            user = authenticate(username=username, password=password) #builtin authentifier l'utilisateur
            if user is not None: #vérifier si l'utilisateur existe

                # login(request, user) #builtin connecter l'utilisateur
                profile_picture_url = None
                if hasattr(user, 'profil_picture') and user.profil_picture:
                    profile_picture_url = request.build_absolute_uri(user.profil_picture.url)
                user_data = { #créer un dictionnaire avec les données de l'utilisateur
                    'message': 'Connexion réussie.', #message de succès
                    'id' : getattr(user, 'id', 'unknown'),
                    'username' : getattr(user, 'username', 'unknown'), #récupérer le username de l'utilisateur
                    'nickname' : getattr(user, 'nickname', 'unknown'), #récupérer le nickname de l'utilisateur
                    'email' : getattr(user, 'email', 'unknown'), #récupérer l'email de l'utilisateur
                    'profil_picture' : profile_picture_url #récupérer l'url de la photo de profil
                }
                return Response(user_data, status=status.HTTP_200_OK) #retourner les données de l'utilisateur
            else:
                return Response({"error": "Nom d'utilisateur ou mot de passe incorrect."}, status=status.HTTP_400_BAD_REQUEST)
        else:
            return Response({"error": serializer.errors}, status=401) #retourner les erreurs de validation
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


@api_view(['POST'])
def add_friend(request):
    try:
        serializer = friendSerializer(data=request.data) #créer une instance de friendSerializer avec les données de la requête
        if serializer.is_valid(): #vérifier si les données sont valides

            friend_data = serializer.validated_data #récupérer les données validées dans friend_data
            username = friend_data['username'] #récupérer le username
            if username == request.user.username: #vérifier si l'utilisateur essaie de s'ajouter lui-même
                return Response({"error": "Vous ne pouvez pas vous ajouter vous-même."}, status=status.HTTP_400_BAD_REQUEST)
            try:
                user_to_add = MyUser.objects.get(username=username) #récupérer l'utilisateur à ajouter
                if user_to_add in request.user.friends.all():
                    return Response({"error": "Cet utilisateur est déjà votre ami."}, status=status.HTTP_400_BAD_REQUEST)
                else:
                    request.user.friends.add(user_to_add) #ajouter l'utilisateur en ami
                    return Response({"message": "Ami ajouté avec succès."}, status=status.HTTP_200_OK)
            except MyUser.DoesNotExist: #si l'utilisateur n'existe pas
                return Response({"error": "Utilisateur introuvable."}, status=status.HTTP_404_NOT_FOUND)
        else: #si les données ne sont pas valides
            return Response({"error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
    except Exception as e: #attraper les exceptions
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


@api_view(['POST'])
def accept_friend_request(request):
	return Response("accept friend request")

@api_view(['POST'])
@permission_classes([IsAuthenticated])
def toggle_2fa(request):
    try:
        request.user.check_2fa = not request.user.check_2fa
        request.user.save()
        if not request.user.check_2fa:
            return Response({"message": "2fa desactivé"}, status=status.HTTP_200_OK)
        else:
            return Response({"message": "2fa activé"}, status=status.HTTP_200_OK)
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)




@ensure_csrf_cookie
def get_csrf_token(request): #obtenir le token csrf
    try:
        csrf_token = get_token(request) #builtin obtenir le token csrf
        return JsonResponse({"csrftoken": csrf_token}, status=status.HTTP_200_OK) #retourner le token csrf
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)



def generate_otp():
    return ''.join(random.choices(string.digits, k=6)) #générer un code otp de 6 chiffres


@api_view(['POST'])
def check_otp_view(request):
    try:
        serializer = otpSerializer(data=request.data) #créer une instance de otpSerializer avec les données de la requête
        if serializer.is_valid(): #vérifier si les données sont valides
            otp_data = serializer.validated_data #récupérer les données validées dans otp_data
            user = MyUser.objects.get(username=otp_data['username']) #récupérer l'utilisateur
            if user.otp_code == otp_data['otp_code']: #vérifier si le code otp est correct
                login(request, user) #builtin connecter l'utilisateur
                user.check_online = True #mettre l'utilisateur en ligne
                user.save()
                token = AccessToken.for_user(user) #générer un token
                encoded_token = str(token) #encoder le token
                refresh = RefreshToken.for_user(user)
                user_data = { #créer un dictionnaire avec les données de l'utilisateur
                    'message': 'Connexion réussie.', #message de succès
                    'username' : getattr(user, 'username', 'unknown'), #récupérer le username de l'utilisateur
                    'nickname' : getattr(user, 'nickname', 'unknown'), #récupérer le nickname de l'utilisateur
                    'email' : getattr(user, 'email', 'unknown'), #récupérer l'email de l'utilisateur
                    'access': encoded_token, #récupérer le token
                    'refresh': str(refresh),
                }
                return Response(user_data, status=status.HTTP_200_OK) #retourner les données de l'utilisateur
            else: #si le code otp est incorrect
                return Response({"error": "Code OTP incorrect."}, status=status.HTTP_400_BAD_REQUEST)
        else: #si les données ne sont pas valides
            return Response({"error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
    except Exception as e: #attraper les exceptions
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


@api_view(['POST'])
@permission_classes([IsAuthenticated])
@permission_classes([MultiPartParser, FormParser])
def uploadpp(request):
     user = request.user

     if "profil_picture" not in request.FILES:
          return JsonResponse({"error": "Aucune image"}, status=status.HTTP_400_BAD_REQUEST)
     
     image = request.FILES["profil_picture"]

     if user.profil_picture:
          old_image_path = os.path.join(settings.MEDIA_ROOT, str(user.profil_picture))
          if os.path.exists(old_image_path):
               os.remove(old_image_path)
     
     user.profil_picture_url = None
     user.profil_picture = image
     user.save()

     return JsonResponse({
          "message": "Profil picture mis a jour !",
          "profil_picture_url": user.profil_picture.url
          }, status=status.HTTP_200_OK)
    # if request.FILES.get('profil_picture'): #vérifier si la méthode est POST et si un fichier est envoyé
    #     user = request.user #récupérer l'utilisateur
    #     user.profil_picture = request.FILES.get('profil_picture') #modifier la photo de profil de l'utilisateur
    #     user.save() #sauvegarder les modifications
    #     return JsonResponse({"message": "Photo de profil mise à jour avec succès."}, status=status.HTTP_200_OK)
    # else: #si la méthode n'est pas POST ou si aucun fichier n'est envoyé
    #     return JsonResponse({"error": "Méthode non autorisée ou Pas d'image envoye"}, status=status.HTTP_400_BAD_REQUEST)




@permission_classes([IsAuthenticated])
class profilView(APIView):
    def get(self, request): #fetch logged in user profil data
        try:
            user = request.user #recupere le user logged in 
            if user is not None: #si le user existe
                stat = user.stat #recupere les Userstat
                friend_list = [] #creer une liste friend vide
                if user.friends.all().count() > 0: #si il a au moins un ami
                    for friend in user.friends.all(): #ajoute les ami un par un
                        # For friends, we also need to check if they have a 42 profile picture
                        friend_pic = None
                        if friend.profil_picture_url:
                            friend_pic = friend.profil_picture_url
                        elif friend.profil_picture:
                            friend_pic = request.build_absolute_uri(friend.profil_picture.url)
                        
                        friend_list.append({
							'username': friend.username,
							'nickname': friend.nickname,
							'ingame': friend.check_ingame,
							'online': friend.check_online,
							'profil_picture': friend_pic,
                        })
                game_list = []  #liste game vide
                if user.games.all().count() > 0: #si au moins une game
                    for game in user.games.all():
                        if game.opponent == 'Tournament': #si les donnees dun tournoi
                            game_list.append({
								'opponent': game.opponent,
								'win': game.win,
								'date': game.date,
                                'tournament_players': game.tournament_players,
                            })
                        else: #sinon
                            game_list.append({
								'opponent': game.opponent,
								'win': game.win,
								'my_score': game.my_score,
								'opponent_score': game.opponent_score,
								'date': game.date,
                            })
                
                # Get profile picture URL - prioritize 42 profile picture if available
                profile_picture_url = None
                if user.profil_picture_url:
                    profile_picture_url = user.profil_picture_url
                elif user.profil_picture:
                    profile_picture_url = request.build_absolute_uri(user.profil_picture.url)
                else:
                    profile_picture_url = request.build_absolute_uri(settings.MEDIA_URL + "PPicture/default.jpg")
                
                data = { #creer un dictionnaire avec les data du user logged in
					'username': user.username,
                    'nickname': user.nickname,
					'email': user.email,
                    'check_2fa': user.check_2fa, #Rajout 2fa
                    'check_oauth': user.check_oauth, #Ajout OAuth status
					'profil_picture': profile_picture_url,
					'number_of_game': stat.number_of_game,
					'number_of_win': stat.number_of_win,
					'number_of_defeat': stat.number_of_defeat,
					'win_percentage': stat.win_percentage,
					'friends': friend_list,
					'games': game_list,
				}
                return JsonResponse({'data': data}, status=status.HTTP_200_OK)
        except Exception as e:
            return JsonResponse({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


    def post(self, request): #fetch another user profil based on username
        try:
            try:
                user = MyUser.objects.get(username=request.data['username']) #recupere le user du username dans la requete
            except MyUser.DoesNotExist: #si le user nexiste pas
                return Response({"error": "Utilisateur non existant."}, status=status.HTTP_400_BAD_REQUEST)
            if user is not None: #si le user existe
                stat = user.stat #recupere les stat
                friend_list = [] #fiend liste vide
                if user.friends.all().count() > 0: #si au moins un ami
                    for friend in user.friends.all(): #ajoute les amis un par un
                        # For friends, we also need to check if they have a 42 profile picture
                        friend_pic = None
                        if friend.profil_picture_url:
                            friend_pic = friend.profil_picture_url
                        elif friend.profil_picture:
                            friend_pic = request.build_absolute_uri(friend.profil_picture.url)
                            
                        friend_list.append({ 
							'username': friend.username,
							'nickname': friend.nickname,
							'ingame': friend.check_ingame,
							'online': friend.check_online,
							'profil_picture': friend_pic,
                        })
                game_list = []
                if user.games.all().count() > 0:
                    for game in user.games.all():
                        if game.opponent == 'Tournament':
                            game_list.append({
								'opponent': game.opponent,
								'win': game.win,
								'date': game.date,
                            })
                        else:
                            game_list.append({
								'opponent': game.opponent,
								'win': game.win,
								'my_score': game.my_score,
								'opponent_score': game.opponent_score,
								'date': game.date,                               
                            })
                
                # Get profile picture URL - prioritize 42 profile picture if available
                profile_picture_url = None
                if user.profil_picture_url:
                    profile_picture_url = user.profil_picture_url
                elif user.profil_picture:
                    profile_picture_url = request.build_absolute_uri(user.profil_picture.url)
                else:
                    profile_picture_url = request.build_absolute_uri(settings.MEDIA_URL + "PPicture/default.jpg")
                
                data = { #build un dictionnaire avec les donnees du user de la requete
					'message': 'Other profil',
					'nickname': user.nickname,
					'email': user.email,
					'profil_picture': profile_picture_url,
					'number_of_game': stat.number_of_game,
					'number_of_win': stat.number_of_win,
					'number_of_defeat': stat.number_of_defeat,
					'win_percentage': stat.win_percentage,
					'friends': friend_list,
					'games': game_list,
                }
                return JsonResponse({'data': data}, status=status.HTTP_200_OK)
        except Exception as e:
            return JsonResponse({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)



@api_view(['POST'])
def game_end_view(request): #fin de game, recupere les donnees de la partie et les sauvegarde, update les stats du user
	try:
		serializer = statsSerializer(data=request.data) #creer une instance stat serializer et recupere les donnees de la requete
		if serializer.is_valid(): #si le donnees sont valides
			game_data = serializer.validated_data #game_data recupere les datas
			user = request.user #recupere le user demandant la requete
			stat = user.stat #recupere les stats du user
			stat.number_of_game += 1
			if game_data['win'] == True:
				stat.number_of_win += 1
			else:
				stat.number_of_defeat += 1
			stat.win_percentage = stat.number_of_win / user.stat.number_of_game * 100 #calcul le taux de victoire
			stat.save()
			user.save()
			new_game = GameStats.objects.create(user=user, win=game_data['win'], opponent=game_data['opponent'], my_score=game_data['my_score'], opponent_score=game_data['opponent_score']) #creer une nouvelle instance gamestats et recupere les donnes de la partie
			new_game.save()
			return Response({'message': 'Data send successfully'}, status=status.HTTP_200_OK)    
		else:
			return Response({"error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
	except Exception as e:
		return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

#The API accepts game data in a POST request.
#The data is validated using a serializer.
#The user’s game stats are updated (number of games, wins, defeats, win percentage).
#A new game record is created and saved.
#A success message is returned, or if an error occurs, an error message is returned.



@api_view(['POST'])
def tournament_view(request):
	try:
		serializer = tournamentSerializer(data=request.data)
		if serializer.is_valid():
			game_data = serializer.validated_data
			user = request.user
			user.stat.number_of_game += 1
			if game_data['win'] == True:
				user.stat.number_of_win += 1
			else:
				user.stat.number_of_defeat += 1
			user.stat.win_percentage = user.stat.number_of_win / user.stat.number_of_game * 100
			user.game = GameStats.objects.create(user=user, win=game_data['win'], opponent=game_data['opponent'])
			user.save()
			return Response({'message': 'Data send successfully'}, status=status.HTTP_200_OK)    
		else:
			return Response({'error': serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
	except Exception as e:
		return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


#The API receives the result of a tournament game via a POST request.
#The data is validated using tournamentSerializer.
#The user's stats (number of games, wins, defeats, win percentage) are updated based on the result.
#A new game record is created for the user, with the tournament as the opponent.
#The updated user data is saved to the database.
#A success message is returned to the client, or an error message is sent if something goes wrong.



@api_view(['GET'])
def ingame_view(request):
	try:
		user = request.user
		user.check_ingame = True
		user.save()
		return Response(status=status.HTTP_200_OK)
	except Exception as e:
		return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


#The view is triggered by a GET request to the ingame_view endpoint.
#The authenticated user is retrieved from the request.
#The user's ingame status is set to True.
#The user object is saved to reflect the change.
#If successful, a 200 OK response is returned. If any error occurs, a 400 Bad Request response with an error message is returned.





@api_view(['GET'])
def not_ingame_view(request):
	try:
		if request.user.is_authenticated:
			user = request.user
			user.check_ingame = False
			user.save()
		return Response(status=status.HTTP_200_OK)
	except Exception as e:
		return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


#The view is triggered by a GET request to the not_ingame_view endpoint.
#The code checks if the user is authenticated.
#If authenticated, the user's ingame status is set to False, indicating they are not currently in a game.
#The updated user object is saved to the database.
#A 200 OK response is returned if everything is successful. If an error occurs, a 400 Bad Request response with an error message is returned.





#Client (Your App) → Requests access to a user’s data.
#Resource Owner (User) → The person who owns the data.
#Authorization Server (e.g., 42 API, Google, GitHub) → Verifies the user and grants access.
#Resource Server → The API that holds the user’s data.
#Redirect URI (Callback URL) → The endpoint where the OAuth provider sends the user back after login.





#user get oauth here and are redirected to the callback view url
# The backend (or frontend) will make a request to https://api.intra.42.fr/oauth/token with:
# client_id
# client_secret
# code
# grant_type=authorization_code
# redirect_uri
# This will return an access token, which allows authenticated API requests.



def oauth2_login(request):
    # get client id form env
    client_id = os.getenv('CLIENT_ID')
    # create full callback url
    redirect_uri = request.build_absolute_uri('/api/callback/')
    # encode url
    encoded_redirect_uri = requests.utils.quote(redirect_uri, safe='')
    
    # build the OAuth authorization URL
    redirect_url = f'https://api.intra.42.fr/oauth/authorize?client_id={client_id}&redirect_uri={encoded_redirect_uri}&response_type=code'
    return redirect(redirect_url)



# Processes the callback from 42, exchanges the authorization code for 
# an access token, retrieves user information, and handles user creation or login.
class OauthCallbackView(APIView): #callback view
    def get(self, request):
        try:
            code = request.GET.get('code') # check if the API42 sent an auth code
            if not code:
                return JsonResponse({"error": "Code d'autorisation manquant"}, status=status.HTTP_400_BAD_REQUEST)
                
            # build the token request data
            data = {
                'grant_type': 'authorization_code',
                'client_id': os.getenv('CLIENT_ID'),
                'client_secret': os.getenv('CLIENT_SECRET'),
                'code': code,
                'redirect_uri': request.build_absolute_uri('/api/callback/'),
            }
            
            # exchange code for an access token
            response = requests.post('https://api.intra.42.fr/oauth/token', data=data)

            if response.status_code != 200:
                return JsonResponse({"error": f"Erreur lors de l'échange du code: {response.text}"}, status=status.HTTP_400_BAD_REQUEST)
                
            access_token = response.json().get('access_token')
            
            # get user data from 42 API
            user_data_response = requests.get(
                'https://api.intra.42.fr/v2/me', 
                headers={'Authorization': f'Bearer {access_token}'}
            )
            
            if user_data_response.status_code != 200:
                return JsonResponse({"error": "Erreur lors de la récupération des données utilisateur"}, status=status.HTTP_400_BAD_REQUEST)
            
            # get full user data from 42 API
            user_data = user_data_response.json()
            
            # extract profile picture URL
            profile_pic_url = None
            if user_data.get('image', {}) and user_data.get('image', {}).get('link'):
                profile_pic_url = user_data.get('image', {}).get('link')
            
            user_serializer = OauthUserSerializer(data=user_data)
            
            if not user_serializer.is_valid():
                return JsonResponse(user_serializer.errors, status=status.HTTP_400_BAD_REQUEST)
                
            login_data = user_serializer.validated_data['login']
            email_data = user_serializer.validated_data['email']
            
            # check if the user already exists
            user_exists = MyUser.objects.filter(username=login_data).exists()
            email_exists = MyUser.objects.filter(email=email_data).exists()
            
            if user_exists:
                # existing user - login
                user = MyUser.objects.get(username=login_data)
                
                # update profile picture URL
                if profile_pic_url and user.check_oauth:
                    user.profil_picture_url = profile_pic_url
                    user.save()
                
                # check if the user has password
                if not user.check_oauth:
                    # user exists but regular account, not OAuth
                    frontend_url = request.build_absolute_uri('/').rstrip('/')
                    error_message = "Un compte avec ce nom d'utilisateur existe déjà. Veuillez vous connecter normalement."
                    encoded_error = requests.utils.quote(error_message)
                    return redirect(f"{frontend_url}/#home?error={encoded_error}")
                
                # If 2fa
                if user.check_2fa:
                    otp = generate_otp_code()
                    user.otp_code = otp
                    user.save()
                    send_otp_email(user.email, otp)
                    
                    # redirect 2fa form
                    frontend_url = request.build_absolute_uri('/').rstrip('/')
                    return redirect(f"{frontend_url}/#home?oauth=true&username={login_data}&otp=true")
                else:
                    # regular login
                    login(request, user)
                    user.check_online = True
                    user.save()
                    
                    # generate JWT token
                    token = AccessToken.for_user(user)
                    refresh = RefreshToken.for_user(user)
                    
                    # redirect to home with token
                    frontend_url = request.build_absolute_uri('/').rstrip('/')
                    return redirect(f"{frontend_url}/#home?token={str(token)}&refresh={str(refresh)}")
                
            elif email_exists:
                # Email already exists in a regular account
                frontend_url = request.build_absolute_uri('/').rstrip('/')
                error_message = "Un compte avec cette adresse email existe déjà. Veuillez vous connecter normalement."
                encoded_error = requests.utils.quote(error_message)
                return redirect(f"{frontend_url}/#home?error={encoded_error}")
            
            else:
                # New user - redirect to password creation page with profile pic URL
                frontend_url = request.build_absolute_uri('/').rstrip('/')
                profile_pic_param = f"&profile_pic={requests.utils.quote(profile_pic_url)}" if profile_pic_url else ""
                return redirect(f"{frontend_url}/#home?oauth=true&username={login_data}&email={email_data}{profile_pic_param}")
        except Exception as e:
            return JsonResponse({'error': str(e)}, status=status.HTTP_400_BAD_REQUEST)
     


#check authorization code
#check oauth response
#check user data validated with serializer
#si le user existe deja dans la DB avec son email ou son username :
# - user existe avec son username et a check oauth true : continue
# - sinon redirige vers le /api/login
#si le user a enable 2fa : the otp code is sent and return to /home
##si le user a disable 2fa : login sans le 2fa et retourne vers /home
#si le user nest pas trouve par username ou email, creer le user et le login, puis redirige vers le /home



@api_view(['POST'])
def oauth2_set_password(request):
    try:
        # Extract data
        data = request.data
        username = data.get('username')
        email = data.get('email')
        password = data.get('password')
        profile_pic_url = data.get('profile_pic')
        
        print(f"Profile pic URL: {profile_pic_url}") #debug
        
        if not all([username, email, password]):
            return Response({"error": "Données manquantes"}, status=status.HTTP_400_BAD_REQUEST)
            
        # check password
        if len(password) < 8:
            return Response({"error": "Le mot de passe doit contenir au moins 8 caractères."}, status=status.HTTP_400_BAD_REQUEST)

        if not re.findall('[A-Z]', password):
            return Response({"error": "Le mot de passe doit contenir au moins une lettre majuscule."}, status=status.HTTP_400_BAD_REQUEST)

        if not re.findall('[a-z]', password):
            return Response({"error": "Le mot de passe doit contenir au moins une lettre minuscule."}, status=status.HTTP_400_BAD_REQUEST)

        if not re.findall('[0-9]', password):
            return Response({"error": "Le mot de passe doit contenir au moins un chiffre."}, status=status.HTTP_400_BAD_REQUEST)
        
        if not re.findall(r'\W', password):
            return Response({"error": "Le mot de passe doit contenir au moins un caractère spécial."}, status=status.HTTP_400_BAD_REQUEST)
            
        # check if user exist
        user_exists = MyUser.objects.filter(username=username).exists()
        
        if user_exists:
            # update existing user
            user = MyUser.objects.get(username=username)

            if not user.check_oauth:
                return Response({
                    "error": "Un compte avec ce nom d'utilisateur existe déjà. Veuillez vous connecter normalement ou utiliser un autre compte 42."
                }, status=status.HTTP_400_BAD_REQUEST)

            user.set_password(password)
            if profile_pic_url:
                user.profil_picture_url = profile_pic_url
            user.save()
        else:
            email_exists = MyUser.objects.filter(email=email).exists()
            if email_exists:
                return Response({
                    "error": "Un compte avec cette adresse email existe déjà. Veuillez vous connecter normalement ou utiliser un autre compte 42."
                }, status=status.HTTP_400_BAD_REQUEST)

            # Create new user
            user = MyUser.objects.create_user(
                username=username,
                email=email,
                password=password,
                nickname=username,
                check_oauth=True
            )
            if profile_pic_url:
                user.profil_picture_url = profile_pic_url
            user.stat = UserStats.objects.create(user=user)
            user.save()
            
        # Log the user
        login(request, user)
        user.check_online = True
        user.save()
        
        # Generate JWT token
        token = AccessToken.for_user(user)
        refresh = RefreshToken.for_user(user)
        
        return Response({
            "message": "Compte créé avec succès",
            "username": username,
            "access": str(token),
            "refresh": str(refresh)
        }, status=status.HTTP_201_CREATED)
        
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)



@api_view(['POST'])
def game_end_other_view(request): #fin de game, recupere les donnees de la partie et les sauvegarde, update les stats du user
    try:
        data = request.data
        username = data.get('username')
        
        #check all required fields
        if not all(key in data for key in ['username', 'opponent', 'win', 'my_score', 'opponent_score']):
            return Response({"error": "Missing required fields"}, status=status.HTTP_400_BAD_REQUEST)
        
        try:
            #get user
            user = MyUser.objects.get(username=username)
        except MyUser.DoesNotExist: #if user not found
            return Response({"error": "User not found"}, status=status.HTTP_404_NOT_FOUND)
        
        # update user stats
        stat = user.stat
        stat.number_of_game += 1
        
        if data['win'] == True:
            stat.number_of_win += 1
        else:
            stat.number_of_defeat += 1
            
        stat.win_percentage = stat.number_of_win / stat.number_of_game * 100
        stat.save()
        
        # create game stats data
        new_game = GameStats.objects.create(
            user=user, 
            win=data['win'], 
            opponent=data['opponent'], 
            my_score=data['my_score'], 
            opponent_score=data['opponent_score']
        )
        new_game.save() #save game data
        
        return Response({'message': 'Data saved for opponent'}, status=status.HTTP_200_OK)
    
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)





@api_view(['POST'])
def tournament_end_view(request): #end tournament send results
    try:
        serializer = TournamentEndSerializer(data=request.data)
        if serializer.is_valid():
            data = serializer.validated_data
            username = data['username']
            win = data['win']
            tournament_players = data['tournament_players']
            
            try:
                user = MyUser.objects.get(username=username)
                
                # update user stats
                stat = user.stat
                stat.number_of_game += 1
                if win:
                    stat.number_of_win += 1
                else:
                    stat.number_of_defeat += 1
                stat.win_percentage = (stat.number_of_win / stat.number_of_game) * 100
                stat.save()
                
                # create tournament data
                # list of players to string 
                players_str = ", ".join(tournament_players)
                
                new_game = GameStats.objects.create(
                    user=user,
                    win=win,
                    opponent="Tournament",  # marker tournament games
                    my_score=None,
                    opponent_score=None,
                    tournament_players=players_str  # save all players from tournament
                )
                new_game.save()
                
                return Response({'message': f'Tournament results saved for {username}'}, status=status.HTTP_200_OK)
                
            except MyUser.DoesNotExist:
                return Response({"error": f"User {username} not found"}, status=status.HTTP_404_NOT_FOUND)
        else:
            return Response({"error": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)
    except Exception as e:
        return Response({"error": str(e)}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)

