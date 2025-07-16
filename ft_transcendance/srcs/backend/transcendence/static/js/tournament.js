
document.addEventListener("tournamentEvent", function(){ // event listener for tournamentEvent
    console.log("Tournament page chargée");
    let tournamentPlayers = [];
    let iaCounter = 1;
    const MAX_PLAYERS = 8;
    window.currentUserPseudoForAddPlayer = null;
    

    window.tournamentState = { //global state of the tournament to track the progress
        inProgress: false, //start with false

        reset: function() { //reset the tournament state
            this.inProgress = false;
            tournamentPlayers = [];
            iaCounter = 1;
            window.tournamentPlayers = [];
            window.currentUserPseudoForAddPlayer = null;
            window.matchInProgress = false;
            
            // reset UI elements
            const playerSection = document.getElementById("playerSection");
            if (playerSection) playerSection.style.display = "block";
            
            const bracketContainer = document.getElementById("bracketContainer");
            if (bracketContainer) {
                bracketContainer.style.display = "none";
                bracketContainer.innerHTML = "";
            }
            
            // clean up any bracket button containers
            const bracketBtnContainers = document.querySelectorAll("#bracketBtnContainer");
            bracketBtnContainers.forEach(container => container.remove());
            
            // clean up any next match buttons
            const nextMatchBtns = document.querySelectorAll("#nextMatchBtn");
            nextMatchBtns.forEach(btn => btn.remove());
            
            const tournament = document.getElementById("tournament");
            if (tournament) tournament.style.display = "none";
            
            const winnerSection = document.getElementById("winnerSection"); 
            if (winnerSection) winnerSection.style.display = "none";
            
            const tournamentDescription = document.getElementById("tournamentDescription");
            if (tournamentDescription) tournamentDescription.style.display = "block";
            
            // reset players list
            const playersList = document.getElementById("playersList");
            if (playersList) playersList.innerHTML = "";
            
            // reset buttons
            const startTournamentBtn = document.getElementById("startTournamentBtn");
            if (startTournamentBtn) {
                startTournamentBtn.disabled = true;
                startTournamentBtn.classList.remove("btn-primary");
                startTournamentBtn.classList.add("btn-secondary");
            }
            
            const addPlayerBtn = document.getElementById("addPlayerBtn");
            if (addPlayerBtn) {
                addPlayerBtn.disabled = false;
                addPlayerBtn.classList.add("btn-primary");
                addPlayerBtn.classList.remove("btn-secondary");
            }
            
            // remove any return buttons
            const returnButtons = document.querySelectorAll("#returnToBracketBtn");
            returnButtons.forEach(btn => btn.remove());
            
            // cancel any ongoing animation frame
            if (window.animationFrameId) {
                cancelAnimationFrame(window.animationFrameId);
                window.animationFrameId = null;
            }
            
            // update UI
            updatePlayersListUI();
            
            console.log("Tournament state has been reset");
        }
    };
    
    // reset tournament state during page load
    window.tournamentState.reset();
  
    window.initTournamentPage = function() {
        console.log("initTournamentPage appelé");
        
        // check if tournament was reset
        if (localStorage.getItem("resetTournament") === "true") {
            window.tournamentState.reset();
            localStorage.removeItem("resetTournament");
        }

        // button "Ajouter un joueur"
        const addPlayerBtn = document.getElementById("addPlayerBtn");
        if (addPlayerBtn) {
            addPlayerBtn.addEventListener("click", function() {
                const addPlayerModalEl = document.getElementById("addPlayerModal");
                if (!addPlayerModalEl) {
                    console.error("addPlayerModal non trouvé");
                    return;
                }
                const addPlayerModal = bootstrap.Modal.getOrCreateInstance(addPlayerModalEl);
                addPlayerModal.show();
            });
        } else {
            console.error("addPlayerBtn non trouvé");
        }

        // player type select
        const playerTypeSelect = document.getElementById("playerTypeSelect");
        if (playerTypeSelect) {
            playerTypeSelect.addEventListener("change", function() {
                const type = this.value;
                const pseudoDiv = document.getElementById("playerPseudoDiv");
                const loginButtonDiv = document.getElementById("loginButtonDiv");
                console.log("Type de joueur sélectionné =", type);
                if (type === "user") {
                    pseudoDiv.style.display = "none";
                    loginButtonDiv.style.display = "block";
                } else if (type === "invité") {
                    pseudoDiv.style.display = "block";
                    loginButtonDiv.style.display = "none";
                } else if (type === "IA") {
                    pseudoDiv.style.display = "none";
                    loginButtonDiv.style.display = "none";
                }
            });
        } else {
            console.error("playerTypeSelect non trouvé");
        }

        // button "Se connecter" modal
        const openLoginForPlayerBtn = document.getElementById("openLoginForPlayerBtn");
        if (openLoginForPlayerBtn) {
            openLoginForPlayerBtn.addEventListener("click", function() {
                console.log("Bouton 'Se connecter' pour ajout de joueur cliqué");
                window.loginPurpose = "addPlayer";
                const loginModalEl = document.getElementById("tournamentLoginModal");
                if (!loginModalEl) {
                    console.error("tournamentLoginModal non trouvé");
                    return;
                }
                const loginModal = bootstrap.Modal.getOrCreateInstance(loginModalEl);
                loginModal.show();
            });
        } else {
            console.error("openLoginForPlayerBtn non trouvé");
        }

        // signin form
        const tournamentLoginForm = document.getElementById("tournamentLoginForm");
        if (tournamentLoginForm) {
            tournamentLoginForm.addEventListener("submit", function(event) {
                event.preventDefault();
                const username = document.getElementById("tournamentUsername").value;
                const password = document.getElementById("tournamentPassword").value;
                fetch('/api/logwithoutlogin/', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                        'X-CSRFToken': getCookie('csrftoken')
                    },
                    credentials: 'same-origin',
                    body: JSON.stringify({ username: username, password: password })
                })
                .then(response => response.json())
                .then(data => {
                    if (data.error) {
                        document.getElementById("tournamentLoginError").classList.remove("d-none");
                    } else {
						localStorage.setItem("currentUsername", data.username);
						localStorage.setItem("currentNickname", data.nickname);
						localStorage.setItem("currentProfilePicture", data.profil_picture);
						if (tournamentPlayers.length >= MAX_PLAYERS) {
							alert(`Le tournoi est limité à ${MAX_PLAYERS} joueurs.`);
							return;
						}
                        const alreadyAdded = tournamentPlayers.some(player => player.name === data.username && player.type === "user");
                        if (!alreadyAdded) {
                            tournamentPlayers.push({ name: data.username, type: "user" });
                            updatePlayersListUI();
                        } else {
                            document.getElementById("login-status").textContent =
                                "L'utilisateur " + data.username + " est déjà inscrit au tournoi.";
                            document.getElementById("login-status").style.display = "block";
                        }
                        // Hide modals
                        const loginModalEl = document.getElementById("tournamentLoginModal");
                        const loginModal = bootstrap.Modal.getOrCreateInstance(loginModalEl);
                        loginModal.hide();
                        const addPlayerModalEl = document.getElementById("addPlayerModal");
                        const addPlayerModal = bootstrap.Modal.getOrCreateInstance(addPlayerModalEl);
                        addPlayerModal.hide();
                    }
                })
                .catch(error => {
                    console.error('Erreur lors de la vérification:', error);
                });
            });
        } else {
            console.error("tournamentLoginForm non trouvé");
        }

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

        // player form function
        const addPlayerForm = document.getElementById("addPlayerForm");
        if (addPlayerForm) {
            addPlayerForm.addEventListener("submit", function(event) {
                event.preventDefault();
                const type = document.getElementById("playerTypeSelect").value;
                let pseudo;

				if (tournamentPlayers.length >= MAX_PLAYERS) {
					alert(`Le tournoi est limité à ${MAX_PLAYERS} joueurs.`);
					return;
				}

                if (type === "invité") {
                    pseudo = document.getElementById("playerPseudoInput").value;
                    if (!pseudo) {
                        alert("Veuillez saisir un pseudo pour le joueur invité.");
                        return;
                    }
					const isDuplicate = tournamentPlayers.some(player => player.name.toLowerCase() === pseudo.toLowerCase());
					if (isDuplicate) {
						alert(`Le pseudo "${pseudo}" est déjà utilisé dans le tournoi.`);
						return;
					}
                } else if (type === "IA") {
                    pseudo = "CPU " + iaCounter;
                    iaCounter++;
                } else if (type === "user") {
                    if (!window.currentUserPseudoForAddPlayer) {
                        alert("Veuillez vous connecter d'abord.");
                        return;
                    }
                    pseudo = window.currentUserPseudoForAddPlayer;
                    window.currentUserPseudoForAddPlayer = null;
                }
                const newPlayer = { name: pseudo, type: type };
                tournamentPlayers.push(newPlayer);
                updatePlayersListUI();
                const addPlayerModalEl = document.getElementById("addPlayerModal");
                const addPlayerModal = bootstrap.Modal.getOrCreateInstance(addPlayerModalEl);
                addPlayerModal.hide();
                addPlayerForm.reset();
                document.getElementById("playerPseudoDiv").style.display = "block";
                document.getElementById("loginButtonDiv").style.display = "none";
            });
        } else {
            console.error("addPlayerForm non trouvé");
        }

        // btton "Commencer le tournoi"
        const startTournamentBtn = document.getElementById("startTournamentBtn");
        if (startTournamentBtn) {
            startTournamentBtn.addEventListener("click", function() {
                if (tournamentPlayers.length < 4) {
                    alert("Il faut au moins 4 joueurs pour lancer le tournoi.");
                    return;
                }
                window.tournamentPlayers = tournamentPlayers;
                window.tournamentState.inProgress = true;
                document.getElementById("playerSection").style.display = "none";
                document.dispatchEvent(new CustomEvent("bracketEvent", {
                    detail: { players: tournamentPlayers }
                }));
            });
        } else {
            console.error("startTournamentBtn non trouvé");
        }
    };
    initTournamentPage();

    // display players list
    function updatePlayersListUI() {
        const playersListDiv = document.getElementById("playersList");
        playersListDiv.innerHTML = "";
        tournamentPlayers.forEach((player, index) => {
            const div = document.createElement("div");
            div.textContent = `Joueur ${index + 1}: ${player.name} (${player.type})`;
            playersListDiv.appendChild(div);
        });

		const addPlayerBtn = document.getElementById("addPlayerBtn");
		if (tournamentPlayers.length >= MAX_PLAYERS) {
			addPlayerBtn.disabled = true;
			addPlayerBtn.classList.add("btn-secondary");
			addPlayerBtn.classList.remove("btn-primary");
		} else {
			addPlayerBtn.disabled = false;
			addPlayerBtn.classList.remove("btn-secondary");
			addPlayerBtn.classList.add("btn-primary");
		}

        const startTournamentBtn = document.getElementById("startTournamentBtn");
        if (tournamentPlayers.length >= 4) {
            startTournamentBtn.disabled = false;
            startTournamentBtn.classList.remove("btn-secondary");
            startTournamentBtn.classList.add("btn-primary");
        } else {
            startTournamentBtn.disabled = true;
            startTournamentBtn.classList.remove("btn-primary");
            startTournamentBtn.classList.add("btn-secondary");
        }
    }

    // optionnal
    function startFullTournament(players) {
        console.log("Lancement du tournoi avec les joueurs :", players);
        document.getElementById("playerSection").style.display = "none";
        document.getElementById("tournament").style.display = "block";
        window.tournamentState.inProgress = true;
    }

    // global expose if needed
    window.updatePlayersListUI = updatePlayersListUI;
    window.startFullTournament = startFullTournament;
});




//submit tournament results
function submitTournamentResults(finalWinner, allPlayers) {
    console.log("Submitting tournament results to all participants");
    
    // get CSRF token for POST reauest
    function getCsrfToken() {
      let cookieValue = null;
      if (document.cookie && document.cookie !== '') {
        const cookies = document.cookie.split(';');
        for (let i = 0; i < cookies.length; i++) {
          const cookie = cookies[i].trim();
          if (cookie.substring(0, 'csrftoken'.length + 1) === ('csrftoken=')) {
            cookieValue = decodeURIComponent(cookie.substring('csrftoken'.length + 1));
            break;
          }
        }
      }
      return cookieValue;
    }
    
    // format list to submit
    const playerNames = allPlayers.map(player => player.name);
    
    // send data to users only
    const userPlayers = allPlayers.filter(player => player.type === "user");
    

    userPlayers.forEach(player => {
      //check winner of the tounament
      const isWinner = (finalWinner.name === player.name);
      
      // prepare data to send
      const tournamentData = {
        username: player.name,
        win: isWinner,
        tournament_players: playerNames, // list of all players
        opponent: "Tournament"  // mark as tournament
      };
      
      // submit to tournament_end API
      fetch('/api/tournament_end/', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'X-CSRFToken': getCsrfToken()
        },
        body: JSON.stringify(tournamentData)
      })
      .then(response => {
        if (!response.ok) {
          throw new Error('Failed to submit tournament results for ' + player.name);
        }
        return response.json();
      })
      .then(data => {
        console.log(`Tournament results submitted for ${player.name}`);
      })
      .catch(error => {
        console.error(`Error submitting tournament results for ${player.name}:`, error);
      });
    });
    
    // set tournament reset flag as complete
    localStorage.setItem("resetTournament", "true");
    
    return true;
  }