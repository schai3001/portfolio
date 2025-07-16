document.addEventListener("profilEvent", function() {
    const profilePicture = document.getElementById("profilePicture");
    const pictureInput = document.getElementById("pictureInput");
    const nicknameSpan = document.getElementById("nickname");
    const emailSpan = document.getElementById("email");
    const editProfileBtn = document.getElementById("editProfileBtn");
    const editPasswordBtn = document.getElementById("editPasswordBtn");
    const editProfileForm = document.getElementById("editProfileForm");
    const editPasswordForm = document.getElementById("editPasswordForm");
    const profileForm = document.getElementById("profileForm");
    const passwordForm = document.getElementById("passwordForm");
    const cancelEdit = document.getElementById("cancelEdit");
    const cancelPassword = document.getElementById("cancelPassword");
    const twoFactorBtn = document.getElementById("twoFactorBtn");
    const profileInfo = document.getElementById("profileInfo");

    let is2FAEnabled = false;
    let currentNickname = "";
    let currentEmail = "";

    // Vérification du token
    const token = localStorage.getItem("access_token");
    if (!token) {
        document.getElementById("app").innerHTML = "<p class='text-danger'>Vous devez être connecté pour voir cette page.</p>";
        return;
    }

    // Charger les infos du profil
    function loadProfile() {
        fetch("/api/profil/", {
            headers: { "Authorization": `Bearer ${token}` }
        })
        .then(response => {
            if (!response.ok){
                return response.json().then(err => {throw err;});
            }
            return response.json();
        })
        .then(data => {
            data = data.data;
            
            // Stocker les valeurs actuelles
            currentNickname = data.nickname || data.username;
            currentEmail = data.email;
            
            // Mettre à jour l'interface
            nicknameSpan.textContent = currentNickname;
            emailSpan.textContent = currentEmail;
            
            if (data.profil_picture) {
                profilePicture.src = data.profil_picture;
            }
            
            is2FAEnabled = data.check_2fa || false;
            console.log("État 2FA chargé:", is2FAEnabled);
            updateTwoFactorButton();
            
            // Mettre à jour les champs du formulaire
            document.getElementById("newNickname").value = currentNickname;
            document.getElementById("newEmail").value = currentEmail;
            
            // Réinitialiser le suivi des modifications
            document.getElementById("newNickname").dataset.changed = "false";
            document.getElementById("newEmail").dataset.changed = "false";
        })
        .catch(() => {
            showError("Erreur de chargement du profil.");
        });
    }
    
    // Mettre à jour le texte et l'apparence du bouton 2FA
    function updateTwoFactorButton() {
        if (!twoFactorBtn) return;
        
        twoFactorBtn.textContent = is2FAEnabled ? "Desactiver 2FA" : "Activer 2FA";
        twoFactorBtn.className = is2FAEnabled 
            ? "btn btn-outline-danger" 
            : "btn btn-outline-success";
    }
    
    // Suivre les modifications des champs
    document.getElementById("newNickname").addEventListener("input", function() {
        this.dataset.changed = (this.value !== currentNickname).toString();
    });
    
    document.getElementById("newEmail").addEventListener("input", function() {
        this.dataset.changed = (this.value !== currentEmail).toString();
    });

    // Gestion du bouton 2FA
    if (twoFactorBtn) {
        twoFactorBtn.addEventListener("click", function() {
            const newState = !is2FAEnabled;
            twoFactorBtn.disabled = true;
            
            fetch("/api/toggle_2fa/", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                    "Authorization": `Bearer ${token}`,
                    "X-CSRFToken": getCookie("csrftoken"),
                },
                body: JSON.stringify({ enable_2fa: newState })
            })
            .then(response => {
                if (!response.ok) {
                    throw new Error("Erreur lors de la modification du 2FA");
                }
                return response.json();
            })
            .then(data => {
                // Mettre à jour l'état local
                is2FAEnabled = newState;
                updateTwoFactorButton();
                
                showSuccess(newState 
                    ? "Authentification à deux facteurs activée avec succès!" 
                    : "Authentification à deux facteurs désactivée avec succès!");
                
                // Si 2FA est activé, montrer les instructions pour la configuration
                if (newState) {
                    showTwoFactorInstructions(data);
                }
            })
            .catch(error => {
                showError(error.message || "Erreur lors de la modification du 2FA");
            })
            .finally(() => {
                // Réactiver le bouton
                twoFactorBtn.disabled = false;
            });
        });
    }
    
    // Afficher les instructions de 2FA après activation
    function showTwoFactorInstructions(data) {
        // Créer modal pour les instructions
        const modalHtml = `
            <div class="modal fade" id="twoFactorModal" tabindex="-1" aria-hidden="true">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <div class="modal-header">
                            <h5 class="modal-title">Configuration de l'authentification à deux facteurs</h5>
                            <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                        </div>
                        <div class="modal-body">
                            <p>Votre authentification à deux facteurs est maintenant activée.</p>
                            <p>Lors de votre prochaine connexion, un code unique sera envoyé à votre adresse email.</p>
                            <div class="alert alert-warning">
                                <p><strong>Important :</strong> Assurez-vous que votre adresse email est correcte et accessible.</p>
                            </div>
                        </div>
                        <div class="modal-footer">
                            <button type="button" class="btn btn-primary" data-bs-dismiss="modal">J'ai compris</button>
                        </div>
                    </div>
                </div>
            </div>
        `;
        
        const modalContainer = document.createElement('div');
        modalContainer.innerHTML = modalHtml;
        document.body.appendChild(modalContainer);
        
        // Afficher modal
        const twoFactorModal = new bootstrap.Modal(document.getElementById('twoFactorModal'));
        twoFactorModal.show();
        
        document.getElementById('twoFactorModal').addEventListener('hidden.bs.modal', function () {
            modalContainer.remove();
        });
    }

    // Gestion de la photo de profil
    pictureInput.addEventListener("change", function(e) {
        const file = e.target.files[0];
        if (file) {
            const formData = new FormData();
            formData.append("profil_picture", file);

            fetch("/api/uploadpp/", {
                method: "POST",
                headers: {
                    "Authorization": `Bearer ${token}`,
                    "X-CSRFToken": getCookie("csrftoken"),
                },
                body: formData
            })
            .then(response => response.json())
            .then(data => {
                profilePicture.src = data.profil_picture_url;
                showSuccess("Photo de profil mise à jour!");
            })
            .catch(() => {
                showError("Erreur lors du téléchargement de l'image.");
            });
        }
    });

    // Gestion du formulaire de modification du profil
    profileForm.addEventListener("submit", function(e) {
        e.preventDefault();
        
        const newNickname = document.getElementById("newNickname");
        const newEmail = document.getElementById("newEmail");
        
        const isNicknameChanged = newNickname.dataset.changed === "true";
        const isEmailChanged = newEmail.dataset.changed === "true";
        
        // Si aucun changement, ne rien faire
        if (!isNicknameChanged && !isEmailChanged) {
            editProfileForm.classList.add("d-none");
            profileInfo.classList.remove("d-none");
            return;
        }
        
        // Préparer les promesses pour les requêtes API
        const promises = [];
        
        // Mettre à jour le nickname si modifié
        if (isNicknameChanged) {
            promises.push(
                fetch("/api/nickname/", {
                    method: "POST",
                    headers: {
                        "Content-Type": "application/json",
                        "Authorization": `Bearer ${token}`,
                        "X-CSRFToken": getCookie("csrftoken"),
                    },
                    body: JSON.stringify({ nickname: newNickname.value })
                })
                .then(response => {
                    if (!response.ok) {
                        throw new Error("Erreur lors de la mise à jour du nickname");
                    }
                    return response.json();
                })
            );
        }
        
        // Mettre à jour l'email si modifié
        if (isEmailChanged) {
            promises.push(
                fetch("/api/change_email/", {
                    method: "POST",
                    headers: {
                        "Content-Type": "application/json",
                        "Authorization": `Bearer ${token}`,
                        "X-CSRFToken": getCookie("csrftoken"),
                    },
                    body: JSON.stringify({ email: newEmail.value })
                })
                .then(response => {
                    if (!response.ok)
                        {
                        return response.json().then(data => {
                            // Créer une erreur avec le message spécifique
                            if (data.error) {
                                throw new Error(data.error);
                            } 
                            else {
                                throw new Error("Erreur lors de la mise à jour de l'email");
                            }
                        });
                    }
                })
            );
        }
        
        // Exécuter toutes les requêtes
        Promise.all(promises)
            .then(() => {
                editProfileForm.classList.add("d-none");
                profileInfo.classList.remove("d-none");

                if (isNicknameChanged) { // Mettre à jour le nickname dans le localStorage
                    const newNicknameValue = document.getElementById("newNickname").value;
                    localStorage.setItem("currentNickname", newNicknameValue);
                }

                loadProfile();  // Recharger le profil
                showSuccess("Profil mis à jour avec succès!");
            })
            .catch(error => {
                showError(error.message);
            });
    });

    // Gestion du formulaire de modification du mot de passe
    passwordForm.addEventListener("submit", function(e) {
        e.preventDefault();
        const currentPassword = document.getElementById("currentPassword").value;
        const newPassword = document.getElementById("newPassword").value;
        const confirmPassword = document.getElementById("confirmPassword").value;

        // Vérifier que les mots de passe correspondent
        if (newPassword !== confirmPassword) {
            showError("Les mots de passe ne correspondent pas.");
            return;
        }

        // Vérifier les règles de complexité du mot de passe
        if (newPassword.length < 8) {
            showError("Le mot de passe doit contenir au moins 8 caractères.");
            return;
        }
    
        if (!/[A-Z]/.test(newPassword)) {
            showError("Le mot de passe doit contenir au moins une lettre majuscule.");
            return;
        }
    
        if (!/[a-z]/.test(newPassword)) {
            showError("Le mot de passe doit contenir au moins une lettre minuscule.");
            return;
        }
    
        if (!/[0-9]/.test(newPassword)) {
            showError("Le mot de passe doit contenir au moins un chiffre.");
            return;
        }
    
        if (!/\W/.test(newPassword)) {
            showError("Le mot de passe doit contenir au moins un caractère spécial.");
            return;
        }

        // Si toutes les validations passent, envoyer la requête
        fetch("/api/change_password", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Authorization": `Bearer ${token}`,
                "X-CSRFToken": getCookie("csrftoken"),
            },
            body: JSON.stringify({
                current_password: currentPassword,
                new_password: newPassword
            })
        })
        .then(response => {
            if (!response.ok) {
                return response.json().then(data => {
                    // Récupérer le message d'erreur spécifique du serveur
                    throw new Error(data.error || "Erreur lors de la modification du mot de passe.");
                });
            }
            return response.json();
        })
        .then(data => {
            editPasswordForm.classList.add("d-none");
            profileInfo.classList.remove("d-none");
            passwordForm.reset();
            showSuccess(data.message || "Mot de passe modifié avec succès!");
        })
        .catch(error => {
            showError(error.message);
        });
    });

    // Toggle des formulaires
    editProfileBtn.addEventListener("click", function() {
        editProfileForm.classList.remove("d-none");
        editPasswordForm.classList.add("d-none");
        profileInfo.classList.add("d-none");
    });

    editPasswordBtn.addEventListener("click", function() {
        editPasswordForm.classList.remove("d-none");
        editProfileForm.classList.add("d-none");
        profileInfo.classList.add("d-none");
    });

    cancelEdit.addEventListener("click", function() {
        editProfileForm.classList.add("d-none");
        profileInfo.classList.remove("d-none");
    });

    cancelPassword.addEventListener("click", function() {
        editPasswordForm.classList.add("d-none");
        profileInfo.classList.remove("d-none");
        passwordForm.reset();
    });

    // Utilitaires pour afficher les messages
    function showError(message) {
        const alert = createAlert(message, 'danger');
        document.querySelector('.card-body').prepend(alert);
    }

    function showSuccess(message) {
        const alert = createAlert(message, 'success');
        document.querySelector('.card-body').prepend(alert);
    }

    function createAlert(message, type) {
        const alert = document.createElement('div');
        alert.className = `alert alert-${type} alert-dismissible fade show`;
        alert.innerHTML = `
            ${message}
            <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
        `;
        setTimeout(() => alert.remove(), 3000);
        return alert;
    }

    loadProfile();
});

function getCookie(name) {
    let cookieValue = null;
    if (document.cookie && document.cookie !== "") {
        const cookies = document.cookie.split(";");
        for (let i = 0; i < cookies.length; i++) {
            const cookie = cookies[i].trim();
            if (cookie.startsWith(name + "=")) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    return cookieValue;
}

