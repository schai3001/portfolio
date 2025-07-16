console.log("Mode Multijoueur chargé !");

//init multi, launched when multi button is clicked
function initMultiplayerMode() {
  console.log("Initializing multiplayer mode");
  
  hideGameElements();
  createPlayerLoginForm();
  showPlayerLoginForm();
  getCurrentUser();
}


function hideGameElements() {
  document.getElementById("game").style.display = "none"; //hide game
  document.getElementById("loadingMessage").style.display = "none"; //hide loading message
}

function createPlayerLoginForm() { //create player login form

  const existingForm = document.getElementById('player2LoginContainer');
  if (existingForm) {
    existingForm.remove(); //remove existing form
  }
  
  // create form
  const player2LoginContainer = document.createElement('div');
  player2LoginContainer.id = 'player2LoginContainer';
  player2LoginContainer.className = 'container mt-4 p-4 border rounded';
  player2LoginContainer.style.display = 'none'; //hide form
  player2LoginContainer.innerHTML = `
    <h3 class="mb-3 text-center">Connexion du Joueur 2</h3>
    <form id="player2LoginForm" class="needs-validation" novalidate>
      <div class="mb-3">
        <label for="player2Username" class="form-label">Nom d'utilisateur</label>
        <input type="text" class="form-control" id="player2Username" required>
      </div>
      <div class="mb-3">
        <label for="player2Password" class="form-label">Mot de passe</label>
        <input type="password" class="form-control" id="player2Password" required>
      </div>
      <div id="player2LoginError" class="alert alert-danger d-none">
        Nom d'utilisateur ou mot de passe incorrect.
      </div>
      <div class="text-center">
        <button type="submit" class="btn btn-primary">Se connecter</button>
        <button type="button" id="cancelPlayer2Login" class="btn btn-secondary ms-2">Annuler</button>
      </div>
    </form>
  `;

  // insert form into DOM
  const gameContainer = document.getElementById('game'); //récupère le container du game
  if (gameContainer && gameContainer.parentNode) {
    gameContainer.parentNode.insertBefore(player2LoginContainer, gameContainer.nextSibling); //insert form
    
    setupFormEventListeners(); //set up event listeners for the form
  } else {
    console.error("Game container not found, cannot add login form");
  }
}


// Function to show the player2 login form
function showPlayerLoginForm() {
  const player2LoginContainer = document.getElementById('player2LoginContainer');
  if (player2LoginContainer) {
    player2LoginContainer.style.display = 'block';
    
    // Reset form fields
    const usernameInput = document.getElementById("player2Username");
    const passwordInput = document.getElementById("player2Password");
    const errorElement = document.getElementById("player2LoginError");
    
    if (usernameInput) usernameInput.value = "";
    if (passwordInput) passwordInput.value = "";
    if (errorElement) errorElement.classList.add("d-none");
  } else {
    console.error("Player 2 login container not found");
  }
}

// Function to set up event listeners for the player2 login form
function setupFormEventListeners() {
    // Cancel button handler
    const cancelButton = document.getElementById("cancelPlayer2Login");
    if (cancelButton) {
      cancelButton.onclick = function() {
        console.log("Cancel button clicked");
        // Hide login form
        const player2LoginContainer = document.getElementById("player2LoginContainer");
        if (player2LoginContainer) {
          player2LoginContainer.style.display = "none";
        }
        
        // Show button container
        const buttonsContainer = document.querySelector(".buttons-container");
        if (buttonsContainer) {
          buttonsContainer.style.display = "flex";
        }
      };
    }
  
    // Player 2 login form submit
    const loginForm = document.getElementById("player2LoginForm");
    if (loginForm) {
      loginForm.onsubmit = function(event) {
        event.preventDefault();
        console.log("Login form submitted");
        
        const username = document.getElementById("player2Username").value;
        const password = document.getElementById("player2Password").value;
        const errorElement = document.getElementById("player2LoginError");
        
        // Reset error message
        if (errorElement) errorElement.classList.add("d-none");
        
        // Validate form fields before submission
        if (!username || !password) {
          errorElement.textContent = "Veuillez remplir tous les champs.";
          errorElement.classList.remove("d-none");
          return;
        }
        
        // Check player 2 login using logwithoutlogin endpoint
        // This endpoint just verifies credentials without changing the session
        fetch('/api/logwithoutlogin/', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
            'X-CSRFToken': getCsrfToken()
          },
          body: JSON.stringify({ username, password })
        })
        .then(response => response.json())
        .then(data => {
          if (data.error) {
            errorElement.textContent = data.error;
            errorElement.classList.remove("d-none");
          } else {
            // Check if player 2 = player 1
            const currentUser = localStorage.getItem("currentUsername");
            if (data.username === currentUser) {
              errorElement.textContent = "Il faut deux joueurs différents pour jouer !";
              errorElement.classList.remove("d-none");
              return;
            }
            
            // Store player 2 info
            const player2Info = {
              username: data.username,
              nickname: data.nickname || data.username,
              id: data.id
            };
            
            // Hide login form
            const player2LoginContainer = document.getElementById("player2LoginContainer");
            if (player2LoginContainer) {
              player2LoginContainer.style.display = "none";
            }
            
            // Start the game with player 2
            startGameMulti(player2Info);
          }
        })
        .catch(error => {
          console.error("Erreur de connexion:", error);
          errorElement.textContent = "Une erreur s'est produite. Veuillez réessayer.";
          errorElement.classList.remove("d-none");
        });
      };
    }
  }

// Helper function to get CSRF token
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

// Get current user info
async function getCurrentUser() {
  const token = localStorage.getItem("access_token");
  
  try {
    const response = await fetch("/api/user/", {
      method: "GET",
      headers: {
        "Authorization": "Bearer " + token
      }
    });
    
    if (!response.ok) {
      throw new Error("Erreur lors de la récupération des informations de l'utilisateur");
    }
    
    const userData = await response.json();
    // Store current username
    localStorage.setItem("currentUsername", userData.username);

    if (userData.nickname) {
      localStorage.setItem("currentNickname", userData.nickname);
    } else {
      localStorage.setItem("currentNickname", userData.username);   
    }

    return userData;
  } catch (error) {
    console.error("Erreur:", error);
    return null;
  }
}

// The startGameMulti function remains the same but is included here for reference
function startGameMulti(player2Info) {
  document.getElementById("game").style.display = "block";
  document.getElementById("loadingMessage").style.display = "none";
  
  // define elements
  const score1Element = document.getElementById('score1');
  const score2Element = document.getElementById('score2');
  const player1Element = document.getElementById('playerA');
  const player2Element = document.getElementById('playerB');
  const winnerElement = document.getElementById('winScreen');
  const countdownElement = document.getElementById('countdown');
  
  // get player 1 data
  const player1Username = localStorage.getItem("currentUsername");
  const player1Nickname = localStorage.getItem("currentNickname") || player1Username;
  
  // set player names
  player1Element.textContent = player1Nickname + " (session owner)";
  player2Element.textContent = player2Info.nickname || player2Info.username;
  
  let turn = localStorage.getItem('turn');
  if (!turn) {
    turn = 1;
    localStorage.setItem('turn', turn);
  }
  else {
    turn++;
    localStorage.setItem('turn', turn);
  }


    // define playground
    const WIDTH = 1000;
    const HEIGHT = 650;
    const fieldWidth = 400, fieldHeight = 200;
  
    // define camera
    const VIEW_ANGLE = 45;
    const ASPECT = WIDTH / HEIGHT;
    const NEAR = 0.1;
    const FAR = 10000;
    let renderer, scene, camera, pointLightOne, pointLightTwo;
  
    // define ball
    const RADIUS = 5;
    const SEGMENTS = 8;
    const RINGS = 8;
  
    // define border
    const barrierThickness = 2;
    const barrierHeight = 5;
    const barrierDepth = fieldHeight;
  
    // define game elements
    let ball, paddle1, paddle2;
    const paddleWidth = 10, paddleHeight = 10;
    const paddleDepth = 50, paddleQuality = 1;
    let paddle1DirZ = 0, paddle2DirZ = 0, paddleSpeed = 2;
    let ballDirX = -1, ballDirZ = 1, ballSpeed = 3;
  
    //define score
    let score1 = 0;
    let score2 = 0;
    let winner = 7;
  
    // define key
    var KEYDOWN = "KeyS", KEYUP = "KeyW", 
    KEYDOWNTWO = "ArrowDown", KEYUPTWO = "ArrowUp";
  
    var goUpOne = false, goDownOne = false, 
    goUpTwo = false, goDownTwo = false;
  
    document.addEventListener('keydown', onKeyDown);
    document.addEventListener('keyup', onKeyUp);
  
    // define game status
    var isRunning = 0;
  
    //countdown
    function startCountdown(callback) {
      countdownElement.style.display = 'block';
      let counter = 3;
      countdownElement.textContent = `Starting in ${counter}`;
  
      const interval = setInterval(() => {
        counter--;
        if (counter > 0) {
          countdownElement.textContent = `Starting in ${counter}`;
        } else {
          clearInterval(interval);
          countdownElement.style.display = 'none';
          callback();
        }
      }, 1000);
    }
  
    //renderer/camera/scene
    renderer = new THREE.WebGLRenderer({ alpha: true});
  
    camera = new THREE.PerspectiveCamera(
      VIEW_ANGLE,
      ASPECT,
      NEAR,
      FAR
    );
  
    scene = new THREE.Scene();
  
    //add camera
    scene.add(camera);
    
    camera.position.set(0, 300, 350);
    camera.lookAt(scene.position);
  
    //start renderer
    renderer.setSize(WIDTH, HEIGHT);
  
    // get elem
    const container = document.getElementById('gameCanvas');
  
    container.appendChild(renderer.domElement);
  
    //light1
    pointLightOne = new THREE.PointLight(0xF8D898);
  
    //light position
    pointLightOne.position.set(100, 100, 0);
    pointLightOne.intensity = 1;
    pointLightOne.distance = 0;
  
    scene.add(pointLightOne);
  
    //light2
    pointLightTwo = new THREE.PointLight(0xF8D898);
  
    pointLightTwo.position.set(-100, 100, 0);
    pointLightTwo.intensity = 1;
    pointLightTwo.distance = 0;
  
    scene.add(pointLightTwo);
  
    //game model
  
    //ball
    const sphereMaterial = new THREE.MeshLambertMaterial({ color: 0x303030 });
  
    ball = new THREE.Mesh(
      new THREE.SphereGeometry(RADIUS, SEGMENTS, RINGS),
      sphereMaterial
    );
  
    ball.position.set(0, RADIUS, 0);
  
    scene.add(ball);
  
    //playground
  
    //surface
    const planeWidth = fieldWidth, planeHeight = fieldHeight, planeQuality = 32;
  
    const planeMaterial = new THREE.MeshLambertMaterial({ color: 0xFFD5A1 });
  
    const plane = new THREE.Mesh(
      new THREE.PlaneGeometry(planeWidth, planeHeight, planeQuality, planeQuality),
      planeMaterial
    );
  
    plane.rotation.x = -Math.PI / 2;
  
    plane.position.y = -5;
  
    scene.add(plane);
  
    //border
  
    const barrierMaterial = new THREE.MeshLambertMaterial({ color: 0x444444 });
  
    //top border
    const topBarrier = new THREE.Mesh(
      new THREE.BoxGeometry(fieldWidth, barrierHeight, barrierThickness),
      barrierMaterial
    );
  
    topBarrier.position.set(0, barrierHeight / 2, -fieldHeight / 2 - barrierThickness / 2);
  
    scene.add(topBarrier);
  
    //bot border
    const bottomBarrier = new THREE.Mesh(
      new THREE.BoxGeometry(fieldWidth, barrierHeight, barrierThickness),
      barrierMaterial
    );
  
    bottomBarrier.position.set(0, barrierHeight / 2, fieldHeight / 2 + barrierThickness / 2);
  
    scene.add(bottomBarrier);
  
    //paddle
    const paddle1Material = new THREE.MeshLambertMaterial({ color: 0x0000FF });
    const paddle2Material = new THREE.MeshLambertMaterial({ color: 0xFF0000 });
  
    //paddle1
    paddle1 = new THREE.Mesh(
      new THREE.BoxGeometry(
        paddleWidth,
        paddleHeight,
        paddleDepth,
        paddleQuality,
        paddleQuality,
        paddleQuality),
        paddle1Material);
  
    scene.add(paddle1);
  
    //paddle2
    paddle2 = new THREE.Mesh(
      new THREE.BoxGeometry(
        paddleWidth,
        paddleHeight,
        paddleDepth,
        paddleQuality,
        paddleQuality,
        paddleQuality),
        paddle2Material);
  
    scene.add(paddle2);
  
    //set paddle position
    paddle1.position.x = -fieldWidth / 2 + paddleWidth;
    paddle2.position.x = fieldWidth / 2 - paddleWidth;
  
    paddle1.position.z = paddleDepth / 2;
    paddle2.position.z = paddleDepth / 2;
  
    //game
    function ballMovement() {
        
      ball.position.x += ballDirX * ballSpeed;
      ball.position.z += ballDirZ * ballSpeed;
  
      if (ball.position.z <= -fieldHeight / 2 + RADIUS) { //hit top
   
        ball.position.z = -fieldHeight / 2 + RADIUS; //reset ball position
 
        ballDirZ = Math.abs(ballDirZ); //change ball direction
      }
      

      if (ball.position.z >= fieldHeight / 2 - RADIUS) { //hit bottom

        ball.position.z = fieldHeight / 2 - RADIUS; //reset ball position

        ballDirZ = -Math.abs(ballDirZ); //change ball direction
      }
  
      if (ballDirZ > ballSpeed * 2) {
        ballDirZ = ballSpeed * 2;
      } else if (ballDirZ < -ballSpeed * 2) {
        ballDirZ = -ballSpeed * 2;
      }
    }
  
    function paddelCollide() {
      //paddle1
      if (ball.position.x - RADIUS <= paddle1.position.x + paddleWidth / 2 
          && ball.position.x + RADIUS >= paddle1.position.x - paddleWidth / 2) {
        if (ball.position.z <= paddle1.position.z + paddleDepth / 2 
          && ball.position.z >= paddle1.position.z - paddleDepth / 2) {
          if (ballDirX < 0) {
            ballDirX = -ballDirX;
            let impactPoint = (ball.position.z - paddle1.position.z) / (paddleDepth / 2);
            ballDirZ = impactPoint * ballSpeed / 1.5;
            ballSpeed += 0.2; // Increase ball speed after collision with paddle1
          }
        }
      }
  
      //paddle2
      if (ball.position.x + RADIUS >= paddle2.position.x - paddleWidth / 2 
          && ball.position.x - RADIUS <= paddle2.position.x + paddleWidth / 2) {
        if (ball.position.z <= paddle2.position.z + paddleDepth / 2 
          && ball.position.z >= paddle2.position.z - paddleDepth / 2) {
          if (ballDirX > 0) {
            ballDirX = -ballDirX;
            let impactPoint = (ball.position.z - paddle2.position.z) / (paddleDepth / 2);
            ballDirZ = impactPoint * ballSpeed / 1.5;
            ballSpeed += 0.2; // Increase ball speed after collision with paddle2
          }
        }
      }
    }
  
    //paddle movement
    function onKeyDown(event) {
      if (event.code == KEYDOWN) {
        goDownOne = true;
      }
      if (event.code == KEYUP) {
        goUpOne = true;
      } 
  
      if (event.code == KEYDOWNTWO) {
        goDownTwo = true;
      }
      if (event.code == KEYUPTWO) {
        goUpTwo = true;
      } 
    }
  
    function onKeyUp(event) {
      if (event.code == KEYDOWN) {
        goDownOne = false;
      }
      if (event.code == KEYUP) {
        goUpOne = false;
      } 
  
      if (event.code == KEYDOWNTWO) {
        goDownTwo = false;
      }
      if (event.code == KEYUPTWO) {
        goUpTwo = false;
      } 
    }
  
    function playersMovement() {
      if (goUpOne && paddle1.position.z > -fieldHeight / 2 + paddleDepth / 2 + 2) {
        paddle1.position.z -= paddleSpeed;
      }
      if (goDownOne && paddle1.position.z < fieldHeight / 2 - paddleDepth / 2 - 2) {
        paddle1.position.z += paddleSpeed;
      }
  
      if (goUpTwo && paddle2.position.z > -fieldHeight / 2 + paddleDepth / 2 + 2) {
        paddle2.position.z -= paddleSpeed;
      }
      if (goDownTwo && paddle2.position.z < fieldHeight / 2 - paddleDepth / 2 - 2) {
        paddle2.position.z += paddleSpeed;
      }
    }
  
    //score
    function goalManagement() {
      if (ball.position.x >= paddle2.position.x + paddleWidth / 2) {
        score1 += 1;
        updateScores();
        resetBall();
      }
  
      if (ball.position.x <= paddle1.position.x - paddleWidth / 2) {
        score2 += 1;
        updateScores();
        resetBall();
      }
    }
  
    function updateScores() {
      if (score1Element && score2Element) {
        score1Element.textContent = score1;
        score2Element.textContent = score2;
      }
    }
  
    function resetBall() {
      ballSpeed = 0;
      ballDirZ = 0;
      ball.position.set(0, RADIUS, 0);
  
      paddle1.position.z = paddleDepth / 2;
      paddle2.position.z = paddleDepth / 2;
      paddle1DirZ = 0;
      paddle2DirZ = 0;
  
      setTimeout(() => {
        ballDirX = (score1 > score2) ? -1 : 1;
        ballDirZ = Math.random() * 2 - 1;
        ballSpeed = 3;
      }, 2000); 
    }
  
    function scoreCheck() {
      if (score1 >= winner) {
        ballSpeed = 0;
        isRunning = 1;
      }
      else if (score2 >= winner) {
        ballSpeed = 0;
        isRunning = 2;
      }
    }
  
    function reloadGamePage() {
      document.removeEventListener('keydown', onKeyDown);
      document.removeEventListener('keyup', onKeyUp);
      
      if (typeof loadPage === 'function') {
        loadPage('game');
      } else {
        location.hash = "game";
        window.dispatchEvent(new Event('hashchange'));
      }
    }
  
    //game update
    function update() {
      renderer.render(scene, camera);
      
      if (isRunning == 3)
      {
        return;
      }
      ballMovement();
      playersMovement();
      paddelCollide();
      goalManagement();
      scoreCheck();
  
      if (localStorage.getItem('turn') != turn)
        return;


      if (isRunning != 0) {
        let win;
        ballSpeed = 0;
        ball.position.set(0, RADIUS, 0);
        renderer.render(scene, camera);
        if (isRunning == 1) {
          winnerElement.textContent = `${player1Nickname} Wins !`;
          win = true;
        }
        else if (isRunning == 2) {
          winnerElement.textContent = `${player2Info.nickname || player2Info.username} Wins !`;
          win = false;
        }
  
        let data = {
          'opponent': player2Info.username,
          'win': win,
          'my_score': score1,
          'opponent_score': score2,
        };
      
        // get CSRF token
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
          
        (async () => {
          try {
            // Get access token for player 1
            let accessToken = localStorage.getItem('access_token');
            let csrfToken = getCsrfToken();
              
            if (!accessToken) {
              console.error('No access token found for player 1');
              return;
            }
            
            // Submit game stats for player 1 (current user)
            const player1Data = {
              'opponent': player2Info.username,
              'win': win,
              'my_score': score1,
              'opponent_score': score2,
            };
            
            console.log("Submitting game data for player 1:", player1Data);
              
            const player1Response = await fetch('/api/game_end/', {
              method: 'POST',
              headers: {
                'Authorization': `Bearer ${accessToken}`,
                'X-CSRFToken': csrfToken,
                'Content-Type': 'application/json',
              },
              body: JSON.stringify(player1Data),
            });
        
            if (!player1Response.ok) {
              console.error('Error submitting player 1 data:', await player1Response.text());
            }
        
            // Now submit game stats for player 2 with the reversed data
            const player2Data = {
              'username': player2Info.username,
              'opponent': player1Username,
              'win': !win, // Opposite of player 1's win status
              'my_score': score2,
              'opponent_score': score1,
            };
            
            console.log("Submitting game data for player 2:", player2Data);
            
            const player2Response = await fetch('/api/game_end_other/', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
                'X-CSRFToken': csrfToken,
              },
              body: JSON.stringify(player2Data),
            });
            
            if (!player2Response.ok) {
              console.error('Error submitting player 2 data:', await player2Response.text());
            } else {
              console.log('Game data successfully submitted for both players');
            }
            
          } catch (error) {
            console.error('Error during stats submission:', error);
          } finally {
            console.log("Retour à la page de jeu dans 5 secondes...");
            setTimeout(reloadGamePage, 5000);
          }
        })();
        return;
      }
      if (location.hash !== "#game")
      {
        isRunning = 3;
        return;
      }
      requestAnimationFrame(update);
    }
  
    startCountdown(() => {
      update();
    });
}

//call when page loaded
initMultiplayerMode();