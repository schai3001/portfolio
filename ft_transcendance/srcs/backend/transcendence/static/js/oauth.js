
// handle Oauth 2.0 with 42 API, manage redirects, user creation and auth process


function checkOAuthRedirect() {
    //analyze URL parameters
    const hash = window.location.hash;
    if (!hash) return;
    
    // remove # and get page name
    const hashParts = hash.substring(1).split('?');
    if (hashParts.length < 2) return; // no parameters
    
    // parse parameters
    const params = new URLSearchParams(hashParts[1]);
    
    // check error message
    if (params.has('error')) {
        return;
    }

    // check oauth register oauth=ture, show password creation
    if (params.has('oauth') && params.get('oauth') === 'true' && params.has('username') && params.has('email')) {
        // get param
        const username = params.get('username');
        const email = params.get('email');
        const profilePic = params.get('profile_pic') || null;
        
        // clear url
        history.replaceState(null, '', `#${hashParts[0]}`);
        
        // show password modal
        showOAuth2PasswordModal(username, email, profilePic);
    } 
    
    // check login with otp
    else if (params.has('oauth') && params.get('oauth') === 'true' && params.has('username') && params.has('otp') && params.get('otp') === 'true') {
        // get username
        const username = params.get('username');
        
        // clear url
        history.replaceState(null, '', `#${hashParts[0]}`);
        
        // show otp modal
        const loginModal = new bootstrap.Modal(document.getElementById("loginModal"));
        loginModal.show();
        
        document.getElementById("username").value = username;
        showOTPField(username);
    }
    
    // check token OAuth
    else if (params.has('token') && params.has('refresh')) {
        // get tokens
        const token = params.get('token');
        const refresh = params.get('refresh');
        
        // clear url
        history.replaceState(null, '', `#${hashParts[0]}`);
        
        // save token
        localStorage.setItem("access_token", token);
        localStorage.setItem("refresh_token", refresh);
        checkLoginStatus();
    }
}


function showOAuth2PasswordModal(username, email, profilePic) {
    // check modal exist
    const modalElement = document.getElementById("oauth2PasswordModal");
    if (!modalElement) {
        console.error("OAuth password modal not found");
        return;
    }
    
    // fill hidden fields
    document.getElementById("oauth2_username").value = username;
    document.getElementById("oauth2_email").value = email;
    
    // add profil picture field
    if (profilePic) {

        let profilePicInput = document.getElementById("oauth2_profile_pic");
        if (!profilePicInput) {
            // create if dont exist
            profilePicInput = document.createElement("input");
            profilePicInput.type = "hidden";
            profilePicInput.id = "oauth2_profile_pic";
            profilePicInput.name = "oauth2_profile_pic";
            modalElement.querySelector("form").appendChild(profilePicInput);
        }
        profilePicInput.value = profilePic;
    }
    
    // show password modal
    const oauth2PasswordModal = new bootstrap.Modal(modalElement);
    oauth2PasswordModal.show();
}


document.addEventListener("DOMContentLoaded", function() {
    // check OAuth parameters on page load
    setTimeout(checkOAuthRedirect, 100);
    
    // event listener password modal
    const oauth2PasswordForm = document.getElementById("oauth2PasswordForm");
    if (oauth2PasswordForm) {
        oauth2PasswordForm.addEventListener("submit", function(e) {
            e.preventDefault();
            
            const username = document.getElementById("oauth2_username").value;
            const email = document.getElementById("oauth2_email").value;
            const password = document.getElementById("oauth2_password").value;
            const passwordConfirm = document.getElementById("oauth2_password_confirm").value;
            const profilePic = document.getElementById("oauth2_profile_pic")?.value || null;
            const errorElement = document.getElementById("oauth2PasswordError");
            
            // reset error message
            errorElement.classList.add("d-none");
            
            // validate passwords same
            if (password !== passwordConfirm) {
                errorElement.textContent = "Les mots de passe ne correspondent pas.";
                errorElement.classList.remove("d-none");
                return;
            }
            
            // get CSRF token
            const csrftoken = getCookie("csrftoken");
            
            // send request to create/update user
            fetch("/api/oauth2/set-password/", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                    "X-CSRFToken": csrftoken
                },
                body: JSON.stringify({
                    username: username,
                    email: email,
                    password: password,
                    profile_pic: profilePic
                })
            })
            .then(response => {
                if (!response.ok) {
                    return response.json().then(data => {
                        throw new Error(data.error || "Erreur lors de la création du compte");
                    });
                }
                return response.json();
            })
            .then(data => {
                // store tokens
                localStorage.setItem("access_token", data.access);
                localStorage.setItem("refresh_token", data.refresh);
                
                // hide modal
                const modal = bootstrap.Modal.getInstance(document.getElementById("oauth2PasswordModal"));
                modal.hide();
                
                // update UI
                checkLoginStatus();
                
                // success message
                const successAlert = document.createElement("div");
                successAlert.className = "alert alert-success alert-dismissible fade show position-fixed top-0 start-50 translate-middle-x mt-3";
                successAlert.style.zIndex = "9999"; // Ensure it's above other elements
                successAlert.style.maxWidth = "90%";
                successAlert.style.width = "auto";
                successAlert.style.boxShadow = "0 4px 6px rgba(0, 0, 0, 0.1)";
                successAlert.innerHTML = `
                    Compte créé avec succès ! Bienvenue, ${data.username}.
                    <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
                `;
                document.body.appendChild(successAlert);
                
                // remove message after 5s
                setTimeout(() => {
                    if (document.body.contains(successAlert)) {
                        successAlert.remove();
                    }
                }, 5000);
            })
            .catch(error => {
                errorElement.textContent = error.message;
                errorElement.classList.remove("d-none");
            });
        });
    }
    
    // listen hash changes , url change after page reload
    window.addEventListener('hashchange', function() {
        setTimeout(checkOAuthRedirect, 100);
    });
});




function getCookie(name) {
    let cookieValue = null;
    if (document.cookie && document.cookie !== "") {
        const cookies = document.cookie.split(";");
        for (let i = 0; i < cookies.length; i++) {
            const cookie = cookies[i].trim();
            if (cookie.substring(0, name.length + 1) === (name + "=")) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    return cookieValue;
}