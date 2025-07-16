document.addEventListener("DOMContentLoaded", function () {
    checkLoginStatus();
    const app = document.getElementById("app");



    document.querySelectorAll("[data-page]").forEach(link => { // get all elements with data-page attribute
        link.addEventListener("click", function (e) { // add click event listener
            e.preventDefault(); // prevent default link behavior
            const page = this.getAttribute("data-page"); // get the page name from the data-page attribute
            
            // check if we're navigating away from the tournament page
            if (location.hash.replace("#", "") === "tournament" && page !== "tournament") {
                // set flag to reset tournament on next visit
                localStorage.setItem("resetTournament", "true");
            }
            
            loadPage(page); // load the page
            history.pushState({ page }, "", `#${page}`); // update history
        });
    });

    // Charger la bonne page si l'utilisateur actualise
    const page = location.hash.replace("#", "") || "home"; // get the page name from the hash
    loadPage(page); // load the page

    // Initialize error handling
    initErrorHandling();
});



window.addEventListener("popstate", function(event) { // listen for back/forward button clicks
    const hash = location.hash.replace("#", ""); // get the page name from the hash
    const page = hash.split('?')[0] || "home"; // remove query string from page name
    
    // check if we're navigating away from the tournament page
    if (document.querySelector('[data-page="tournament"].active') && page !== "tournament") {
        // set flag to reset tournament on next visit
        localStorage.setItem("resetTournament", "true");
    }
    
    loadPage(page);
});


// load content dynamically without refreshing pages

function loadPage(page) {
    // get the base page name
    const pageName = page.split('?')[0]; // remove query string from page name
    
    // use fetch API to load HTML content from server
    fetch(`static/pages/${pageName}.html`) // fetch the page content
        .then(response => response.text())
        .then(html => {
            const app = document.getElementById("app"); // injects the HTML into the app container
            app.innerHTML = html; // set the page content
            const event = new CustomEvent(`${pageName}Event`, { // create a custom event
                detail: { message: `La page ${pageName} est chargée !` } // add a message to the event
            });
            document.dispatchEvent(event);
            console.log(`trying to send ${pageName}Event`);
            
            // check for OAuth parameters and errors after page is loaded
            setTimeout(() => {
                if (page.includes('oauth=true')) { // check if page contains OAuth parameters
                    checkOAuthRedirect();
                }
            }, 100);
        })
        .catch(() => {
            const app = document.getElementById("app");
            app.innerHTML = "<p class='text-danger'>Erreur de chargement...</p>";
        });
}


// global function to show error messages
window.showErrorMessage = function(message) {
    // remove existing error messages before
    document.querySelectorAll('.error-overlay-message').forEach(el => el.remove());
    
    // create error message elem
    const errorAlert = document.createElement("div");
    errorAlert.className = "error-overlay-message alert alert-danger alert-dismissible fade show";
    
    errorAlert.innerHTML = `
        ${message}
        <button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button>
    `;
    
    document.body.appendChild(errorAlert);
    
    ///remove after 5s
    setTimeout(() => {
        if (document.body.contains(errorAlert)) {
            errorAlert.remove();
        }
    }, 5000);
};


// function to check URL for error parameters
function checkUrlForErrors() {
    // get URL parameters
    const hashParts = window.location.hash.split('?');
    if (hashParts.length < 2) return false;
    
    const params = new URLSearchParams(hashParts[1]);
    if (params.has('error')) {
        const errorMessage = decodeURIComponent(params.get('error'));
        
        // show error message
        window.showErrorMessage(errorMessage);
        
        // clear URL
        history.replaceState(null, '', `#${hashParts[0]}`);
        return true;
    }
    
    return false;
}

// init error handling
function initErrorHandling() {
    // check for error on page load
    checkUrlForErrors();
    
    // listen hashchange
    window.addEventListener('hashchange', function() {
        setTimeout(checkUrlForErrors, 100);
    });
}