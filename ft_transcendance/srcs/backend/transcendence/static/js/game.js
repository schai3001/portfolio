document.addEventListener("gameEvent", function(){ //event listener gameEvent
  console.log("Game page chargée");
  const token = localStorage.getItem("access_token"); //récupère le token
  if (!token) { //si pas de token
    document.getElementById("app").innerHTML = "<p class='text-danger'>Vous devez être connecté pour voir cette page.</p>"; //affiche message d'erreur
    return;
  }

  refreshCurrentUser(); //resfresh user data
  
  
  document.getElementById("game").style.display = "none"; //hide game
  document.getElementById("loadingMessage").style.display = "none"; //hide loading message
  
 
  const buttonsContainer = document.querySelector(".buttons-container"); //récupère le container des boutons
  if (buttonsContainer) {
    buttonsContainer.style.display = "flex"; //affiche les boutons game
  }
  

  const soloBtn = document.getElementById("soloBtn"); //récupère le bouton solo
  if (soloBtn) { //if solo button exist
    soloBtn.onclick = function() { //click solo button
      console.log("Solo button clicked");
      // Hide buttons container
      const buttonsContainer = document.querySelector(".buttons-container"); //récupère le container des boutons
      if (buttonsContainer) {
        buttonsContainer.style.display = "none"; //hide les boutons
      }
      
      // hide player2 container if it exist
      const player2LoginContainer = document.getElementById("player2LoginContainer"); //récupère le container du player2
      if (player2LoginContainer) {
        player2LoginContainer.style.display = "none"; //hide player2 container
      }
      
      document.getElementById("game").style.display = "block"; //show game
      document.getElementById("loadingMessage").style.display = "block"; //show loading message
      
      startGame("solo");
    };
  }
  
  const multiBtn = document.getElementById("multiBtn"); //récupère le bouton multi
  if (multiBtn) {
    multiBtn.onclick = function() {
      console.log("Multi button clicked");

      loadMultiplayerMode(); //load multiplayer mode
    };
  }
});


// retrieve user data for nickanme and username
async function refreshCurrentUser() { //refresh user data
  const token = localStorage.getItem("access_token");
  
  try {
    const response = await fetch("/api/user/", { //fetch user data
      method: "GET",
      headers: {
        "Authorization": "Bearer " + token
      }
    });
    
    if (!response.ok) {
      throw new Error("Erreur lors de la récupération des informations de l'utilisateur");
    }
    
    const userData = await response.json(); //get user data
    

    localStorage.setItem("currentUsername", userData.username); //store username in localstorage
    

    if (userData.nickname) {
      localStorage.setItem("currentNickname", userData.nickname); //store nickname in localstorage
    } else {
      localStorage.setItem("currentNickname", userData.username);
    }
    
    return userData;
  } catch (error) {
    console.error("Erreur lors du rafraîchissement des données utilisateur:", error);
    return null;
  }
}





function loadMultiplayerMode() { //load multiplayer mode
  console.log("Loading multiplayer mode");
  
  const buttonsContainer = document.querySelector(".buttons-container");
  if (buttonsContainer) {
    buttonsContainer.style.display = "none"; //hide buttons container
  }
  
 
  const loadingMessage = document.getElementById("loadingMessage");
  loadingMessage.innerHTML = "<p>Chargement du mode <strong>multijoueur</strong>...</p>";
  loadingMessage.style.display = "block"; //show loading message
  
  //dynamic script loading
  let multiScript = document.querySelector('script[src="static/js/multi.js"]'); //check if multi.js already loaded
  if (!multiScript) {
    multiScript = document.createElement("script"); //create script
    multiScript.src = "static/js/multi.js"; //script source
    multiScript.onload = function() { //load script multiplayer
      console.log("multi.js loaded successfully");
    };
    document.body.appendChild(multiScript); //add script to body
  } else {

    console.log("multi.js already loaded, initializing directly");
    if (typeof initMultiplayerMode === 'function') {
      initMultiplayerMode(); //initialize multiplayer mode
    } else {
      console.error("initMultiplayerMode function not found!");
    }
  }
}





function startGame(mode){ //start game
  console.log(`start mode ${mode}`);
  const gameContainer = document.getElementById("gameContainer");
  document.getElementById("loadingMessage").innerHTML = `<p>Chargement du mode <strong>${mode}</strong>...</p>`;
  document.getElementById("loadingMessage").style.display = "block";
  

  document.querySelectorAll("script[data-game]").forEach(s => s.remove()); //remove all game scripts
  

  const script = document.createElement("script");
  script.src = `static/js/${mode}.js`;
  console.log(`start script ${mode}`);
  script.defer = true;
  script.dataset.game = "true"; // Marque le script pour le retrouver
  document.body.appendChild(script); //add script to body
}