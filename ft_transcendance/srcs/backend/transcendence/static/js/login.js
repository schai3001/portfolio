
document.addEventListener("DOMContentLoaded", function () {

    const loginForm = document.getElementById("loginForm"); //get the login form
    const loginBtn = document.getElementById("loginBtn"); //get the login button
    const logoutBtn = document.getElementById("logoutBtn"); //get the logout button
    const loginModal = new bootstrap.Modal(document.getElementById("loginModal")); //get the login modal
    const profilBtn = document.getElementById("profilBtn"); //get the profil button
    const gameBtn = document.getElementById("gameBtn"); //get the game button
    const tournamentBtn = document.getElementById("tournamentBtn"); //get the tournament button
    const otpForm = document.getElementById("otpForm"); //get the OTP form


    const loginModalBody = document.querySelector("#loginModal .modal-body"); //get the login modal body

    if (loginModalBody) { //if the login modal body exists

        if (!document.getElementById("login42Btn")) { //if the 42 login button does not exist

            const separator = document.createElement("hr"); //create a separator for the login buttons
            separator.className = "mt-3"; //add a class to the separator
            separator.id = "oauth-separator"; //add an id to the separator
            
            const oauthContainer = document.createElement("div"); //create a container for the login buttons
            oauthContainer.className = "mt-3 text-center"; //add a class to the container
            oauthContainer.id = "oauth-container"; //add an id to the container
            

            const ouText = document.createElement("p"); //create a paragraph for the "ou" text
            ouText.textContent = "ou";
            

            const oauthButton = document.createElement("a"); //create a button for the 42 login
            oauthButton.href = "/api/callin/";
            oauthButton.className = "btn btn-outline-secondary";
            oauthButton.id = "login42Btn";
            oauthButton.textContent = "Se connecter avec 42";
            

            oauthContainer.appendChild(ouText);
            oauthContainer.appendChild(oauthButton);
            

            loginForm.after(separator);
            separator.after(oauthContainer);
        }
    }

    loginForm.addEventListener("submit", function (e) { //when the login form is submitted
        e.preventDefault();
        const username = document.getElementById("username").value; //get the username
        const password = document.getElementById("password").value; //get the password

        fetch("/api/login/", { //fetch the login endpoint
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "X-CSRFToken": getCookie("csrftoken"), // Fetch CSRF token from cookies
            },
            body: JSON.stringify({username, password})
        })
        .then(response => response.json())
        .then(data => {
            if (data.otp) { // If OTP is required
                showOTPField(data.username); // Show OTP field
            }
            else if (data.access) {
                localStorage.setItem("access_token", data.access);
                localStorage.setItem("refresh_token", data.refresh)
                checkLoginStatus();
                loginModal.hide();
            } else {
                document.getElementById("loginError").classList.remove("d-none");
            }
        })
        .catch(() =>{
            document.getElementById("loginError").classList.remove("d-none");
        });
    });


    otpForm.addEventListener("submit", function(e) {
        e.preventDefault();
        const username = document.getElementById("username").value;
        const otp_code = document.getElementById("otp_code").value;
        fetch("/api/check_2fa/", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "X-CSRFToken": getCookie("csrftoken"),
            },
            body: JSON.stringify({ username, otp_code })
        })
        .then(response => response.json())
        .then(data => {
            if (data.access) {
                localStorage.setItem("access_token", data.access);
                localStorage.setItem("refresh_token", data.refresh);
                
                // first reset the OTP field
                resetOTPField();
                
                // update the UI based on login status
                checkLoginStatus();
                
                // get a fresh instance of the modal
                const loginModalElement = document.getElementById("loginModal");
                const loginModalInstance = bootstrap.Modal.getInstance(loginModalElement);
                
                // close the modal 
                if (loginModalInstance) {
                    loginModalInstance.hide();
                } else {
                    console.log("Modal instance not found, trying direct Bootstrap method");
                    const loginModal = new bootstrap.Modal(loginModalElement);
                    loginModal.hide();
                }
                
                console.log("Login modal should be hidden now");
            } else {
                document.getElementById("loginError").classList.remove("d-none");
            }
        })
        .catch(() => {
            document.getElementById("loginError").classList.remove("d-none");
        });
    });



    function redirectToHome()
    {
        if (location.hash !== "#home" && location.hash !== "")
        {
            location.hash = "home";
            const event = new CustomEvent('hashchange');
            window.dispatchEvent(event);
        }
    }

    function checkLoginStatus() {
        const token = localStorage.getItem("access_token");
        if (!token) {
            updateUI(false);
            return;
        }
        fetch("/api/user/", { // Endpoint Django pour vérifier le token
            method: "GET",
            headers: {
                "Authorization": "Bearer " + token
            }
        })
        .then(response => {
            if (response.status === 401) {
                refreshToken();
                throw new Error("Token expiré");
            }
            return response.json();
        })
        .then(data => {
            console.log("Utilisateur connecté :", data);
            updateUI(true);
        })
        .catch(error => {
            console.warn("Non connecté :", error);
            localStorage.removeItem("access_token"); // Supprime le token s'il est invalide
            updateUI(false);
        });
    }

    function updateUI(isLoggedIn) {
        const registerBtn = document.getElementById("registerBtn");
        const friendsButtonContainer = document.getElementById("friendsButtonContainer");
    
        if (isLoggedIn) {
            loginBtn.classList.add("d-none");
            registerBtn.classList.add("d-none");
            logoutBtn.classList.remove("d-none");   
            profilBtn.classList.remove("d-none");
            gameBtn.classList.remove("d-none");
            tournamentBtn.classList.remove("d-none");
        
            // Afficher la friend list quand on est connecté
            if (friendsButtonContainer) {
                friendsButtonContainer.classList.remove("d-none");
                // Si une fonction loadFriendsList existe (définie dans friendslist.js)
                if (typeof window.loadFriendsList === 'function') {
                    window.loadFriendsList();
                }
            }
        } else {
            loginBtn.classList.remove("d-none");
            registerBtn.classList.remove("d-none");
            logoutBtn.classList.add("d-none");
            profilBtn.classList.add("d-none");
            gameBtn.classList.add("d-none");
            tournamentBtn.classList.add("d-none");
        
            // Cacher la friend list quand on est déconnecté
            if (friendsButtonContainer) {
                friendsButtonContainer.classList.add("d-none");
                const friendsSidebar = document.getElementById("friendsSidebar");
                if (friendsSidebar) {
                    friendsSidebar.classList.add("d-none");
                }
            }
        }
    }
    // rend checkLoginStatus global
    window.checkLoginStatus = checkLoginStatus;
    window.updateUI = updateUI;
    
    logoutBtn.addEventListener("click", function ()
    {
        fetch("/api/logout/", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "X-CSRFToken": getCookie("csrftoken"), // Fetch CSRF token from cookies
            },
        })
        .then(response => {
            if (!response.ok){
                throw new Error("probably not connected");
            }
            return response.json();
        })
        .then(data => {
                console.log(data.message);
                localStorage.removeItem("access_token");
                localStorage.removeItem("refresh_token");
                resetOTPField();
                checkLoginStatus();
                redirectToHome();
        })
        .catch(error => console.error("Error :", error));
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

    // execute checkLoginStatus au chargement de la page
    checkLoginStatus();
    resetOTPField();
});

function showOTPField(username) {
    console.log("hide !");
    document.getElementById("password").classList.add("d-none");
    document.getElementById("username").classList.add("d-none");
    document.getElementById("usernameLabel").classList.add("d-none");
    document.getElementById("passwordLabel").classList.add("d-none");
    document.getElementById("btnlogin").classList.add("d-none");
    document.getElementById("otpForm").classList.remove("d-none");
    document.getElementById("username").value = username; // Préserve l'username
    
    // hide OAuth login button and separator when showing OTP form
    const oauthContainer = document.getElementById("oauth-container");
    const oauthSeparator = document.getElementById("oauth-separator");
    if (oauthContainer) oauthContainer.classList.add("d-none");
    if (oauthSeparator) oauthSeparator.classList.add("d-none");
}

function resetOTPField() {
    document.getElementById("password").classList.remove("d-none");
    document.getElementById("username").classList.remove("d-none");
    document.getElementById("usernameLabel").classList.remove("d-none");
    document.getElementById("passwordLabel").classList.remove("d-none");
    document.getElementById("btnlogin").classList.remove("d-none");
    document.getElementById("otpForm").classList.add("d-none");
    
    // show OAuth login button and separator again when resetting to regular login
    const oauthContainer = document.getElementById("oauth-container");
    const oauthSeparator = document.getElementById("oauth-separator");
    if (oauthContainer) oauthContainer.classList.remove("d-none");
    if (oauthSeparator) oauthSeparator.classList.remove("d-none");
}

document.getElementById("loginModal").addEventListener("hidden.bs.modal", function () {
    resetOTPField();
});