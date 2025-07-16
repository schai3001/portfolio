// tournamentGame.js

function startGameTournament(playername1, playername2, type1, type2) {
	console.log("TournamentGame page chargée");
	document.getElementById("bracketContainer").style.display = "none";
	document.getElementById("bracketBtnContainer").style.display = "none";
	document.getElementById("tournament").style.display = "block";
	document.getElementById("gameContainer").style.display = "block";
	
	window.matchInProgress = true;
	
	return new Promise((resolve) => {
	  const container = document.getElementById('gameCanvas');
	  container.innerHTML = ''; // canvas reset
	
	  const score1Element = document.getElementById('score1');
	  const score2Element = document.getElementById('score2');
	  const player1Element = document.getElementById('playerA');
	  const player2Element = document.getElementById('playerB');
	  const winnerElement = document.getElementById('winScreen');
	  const countdownElement = document.getElementById('countdown');
	
	  countdownElement.style.display = 'block';
	
	  const WIDTH = 1000;
	  const HEIGHT = 650;
	  const fieldWidth = 400, fieldHeight = 200;
	
	  const VIEW_ANGLE = 45;
	  const ASPECT = WIDTH / HEIGHT;
	  const NEAR = 0.1;
	  const FAR = 10000;
	  let renderer, scene, camera, pointLightOne, pointLightTwo;
	
	  const RADIUS = 5, SEGMENTS = 16, RINGS = 16;
	  const barrierThickness = 2, barrierHeight = 5, barrierDepth = fieldHeight;
	
	  let ball, paddle1, paddle2;
	  const paddleWidth = 10, paddleHeight = 10;
	  const paddleDepth = 50, paddleQuality = 1;
	  let paddle1DirZ = 0, paddle2DirZ = 0, paddleSpeed = 2;
	  let ballDirX = -1, ballDirZ = 1, ballSpeed = 3;
	
	  let score1 = 0, score2 = 0;
	  let winningScore = 3;
	  let isRunning = 0;
	  const KEYDOWN = "KeyS", KEYUP = "KeyW", KEYDOWNTWO = "ArrowDown", KEYUPTWO = "ArrowUp";
	  let goUpOne = false, goDownOne = false, goUpTwo = false, goDownTwo = false;
	
	  document.addEventListener('keydown', onKeyDown);
	  document.addEventListener('keyup', onKeyUp);
	
	  player1Element.textContent = playername1;
	  player2Element.textContent = playername2;
  
	  const isAI1 = (type1 === "IA");
	  const isAI2 = (type2 === "IA");
	
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
  
	  renderer = new THREE.WebGLRenderer({ alpha: true });
	  camera = new THREE.PerspectiveCamera(VIEW_ANGLE, ASPECT, NEAR, FAR);
	  scene = new THREE.Scene();
	  scene.add(camera);
	  camera.position.set(0, 300, 350);
	  camera.lookAt(scene.position);
	  renderer.setSize(WIDTH, HEIGHT);
	  container.appendChild(renderer.domElement);
  
	  pointLightOne = new THREE.PointLight(0xF8D898);
	  pointLightOne.position.set(100, 100, 0);
	  pointLightOne.intensity = 1;
	  scene.add(pointLightOne);
  
	  pointLightTwo = new THREE.PointLight(0xF8D898);
	  pointLightTwo.position.set(-100, 100, 0);
	  pointLightTwo.intensity = 1;
	  scene.add(pointLightTwo);
  
	  const sphereMaterial = new THREE.MeshLambertMaterial({ color: 0x303030 });
	  ball = new THREE.Mesh(new THREE.SphereGeometry(RADIUS, SEGMENTS, RINGS), sphereMaterial);
	  ball.position.set(0, RADIUS, 0);
	  scene.add(ball);
  
	  const planeMaterial = new THREE.MeshLambertMaterial({ color: 0xFFD5A1 });
	  const plane = new THREE.Mesh(new THREE.PlaneGeometry(fieldWidth, fieldHeight, 32, 32), planeMaterial);
	  plane.rotation.x = -Math.PI / 2;
	  plane.position.y = -5;
	  scene.add(plane);
  
	  const barrierMaterial = new THREE.MeshLambertMaterial({ color: 0x444444 });
	  const topBarrier = new THREE.Mesh(new THREE.BoxGeometry(fieldWidth, barrierHeight, barrierThickness), barrierMaterial);
	  topBarrier.position.set(0, barrierHeight / 2, -fieldHeight / 2 - barrierThickness / 2);
	  scene.add(topBarrier);
  
	  const bottomBarrier = new THREE.Mesh(new THREE.BoxGeometry(fieldWidth, barrierHeight, barrierThickness), barrierMaterial);
	  bottomBarrier.position.set(0, barrierHeight / 2, fieldHeight / 2 + barrierThickness / 2);
	  scene.add(bottomBarrier);
  
	  const paddle1Material = new THREE.MeshLambertMaterial({ color: 0x0000FF });
	  const paddle2Material = new THREE.MeshLambertMaterial({ color: 0xFF0000 });
	  paddle1 = new THREE.Mesh(new THREE.BoxGeometry(paddleWidth, paddleHeight, paddleDepth, paddleQuality, paddleQuality, paddleQuality), paddle1Material);
	  scene.add(paddle1);
	  paddle2 = new THREE.Mesh(new THREE.BoxGeometry(paddleWidth, paddleHeight, paddleDepth, paddleQuality, paddleQuality, paddleQuality), paddle2Material);
	  scene.add(paddle2);
	  paddle1.position.x = -fieldWidth / 2 + paddleWidth;
	  paddle2.position.x = fieldWidth / 2 - paddleWidth;
	  paddle1.position.z = paddleDepth / 2;
	  paddle2.position.z = paddleDepth / 2;
  
	  function ballMovement() {
		ball.position.x += ballDirX * ballSpeed;
		ball.position.z += ballDirZ * ballSpeed;
		if (ball.position.z <= -fieldHeight / 2 + RADIUS || ball.position.z >= fieldHeight / 2 - RADIUS) {
		  ballDirZ = -ballDirZ;
		}
		if (ballDirZ > ballSpeed * 2) ballDirZ = ballSpeed * 2;
		if (ballDirZ < -ballSpeed * 2) ballDirZ = -ballSpeed * 2;
	  }
  
	  function paddleCollide() {
		if (ball.position.x - RADIUS <= paddle1.position.x + paddleWidth / 2 &&
			ball.position.x + RADIUS >= paddle1.position.x - paddleWidth / 2) {
		  if (ball.position.z <= paddle1.position.z + paddleDepth / 2 &&
			  ball.position.z >= paddle1.position.z - paddleDepth / 2) {
			if (ballDirX < 0) {
			  ballDirX = -ballDirX;
			  let impactPoint = (ball.position.z - paddle1.position.z) / (paddleDepth / 2);
			  ballDirZ = impactPoint * ballSpeed / 1.5;
			  ballSpeed += 0.2;
			}
		  }
		}
		if (ball.position.x + RADIUS >= paddle2.position.x - paddleWidth / 2 &&
			ball.position.x - RADIUS <= paddle2.position.x + paddleWidth / 2) {
		  if (ball.position.z <= paddle2.position.z + paddleDepth / 2 &&
			  ball.position.z >= paddle2.position.z - paddleDepth / 2) {
			if (ballDirX > 0) {
			  ballDirX = -ballDirX;
			  let impactPoint = (ball.position.z - paddle2.position.z) / (paddleDepth / 2);
			  ballDirZ = impactPoint * ballSpeed / 1.5;
			  ballSpeed += 0.2;
			}
		  }
		}
	  }
  
	  function onKeyDown(event) {
		if (event.code === KEYDOWN) goDownOne = true;
		if (event.code === KEYUP) goUpOne = true;
		if (event.code === KEYDOWNTWO) goDownTwo = true;
		if (event.code === KEYUPTWO) goUpTwo = true;
	  }
  
	  function onKeyUp(event) {
		if (event.code === KEYDOWN) goDownOne = false;
		if (event.code === KEYUP) goUpOne = false;
		if (event.code === KEYDOWNTWO) goDownTwo = false;
		if (event.code === KEYUPTWO) goUpTwo = false;
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

	  

	function moveAIForPaddle2() {
		// initialize properties
		if (!paddle2.lastUpdateTime) {
			paddle2.lastUpdateTime = null;
			paddle2.targetPosition = 0;
			paddle2.previousTarget = 0;
			paddle2.currentTarget = 0;
			paddle2.interpolationStart = 0;
			paddle2.interpolationDuration = 300;
			paddle2.isMoving = false;
			paddle2.reactionDelay = 200;
		}

		const currentTime = Date.now();
		if (!paddle2.lastUpdateTime || currentTime - paddle2.lastUpdateTime > 1000) {
			// calculate target position
			paddle2.targetPosition = calculateTargetPosition(paddle2, true);

			const edgeBuffer = paddleDepth / 2 + 5;
			const topLimit = -fieldHeight / 2 + edgeBuffer;
			const bottomLimit = fieldHeight / 2 - edgeBuffer;
			
			paddle2.targetPosition = Math.max(topLimit, Math.min(bottomLimit, paddle2.targetPosition));
			
			paddle2.lastUpdateTime = currentTime;
			paddle2.reactionDelay = Math.random() * 80 + 50;
			
			paddle2.previousTarget = paddle2.currentTarget || paddle2.targetPosition;
			paddle2.currentTarget = paddle2.targetPosition;
			paddle2.interpolationStart = currentTime + paddle2.reactionDelay;
			paddle2.interpolationDuration = 300 + Math.random() * 250;
			
			paddle2.isMoving = true;
		}
		
		if (currentTime > paddle2.interpolationStart) {
			const elapsed = currentTime - paddle2.interpolationStart;
			const progress = Math.min(elapsed / paddle2.interpolationDuration, 1);
			
			// smooth movement
			const easedProgress = progress < 0.5
				? 2 * progress * progress
				: 1 - Math.pow(-2 * progress + 2, 2) / 2;
			
			const interpolatedTarget = paddle2.previousTarget + 
				(paddle2.currentTarget - paddle2.previousTarget) * easedProgress;
			
			const distanceToTarget = interpolatedTarget - paddle2.position.z;
			const absDistance = Math.abs(distanceToTarget);
			
			const deadZone = 1.5;
			
			if (progress >= 1.0 && absDistance <= deadZone) {
				paddle2.isMoving = false;
			}
			
			if (paddle2.isMoving && absDistance > deadZone) {
				const baseSpeed = paddleSpeed * 0.9;
				
				const speedFactor = Math.min(1.0, absDistance / 15);
				const finalSpeed = baseSpeed * speedFactor;
				
				if (Math.random() < 0.05) {
					paddle2.position.z += (distanceToTarget > 0) ? -finalSpeed : finalSpeed;
				} else {
					paddle2.position.z += (distanceToTarget > 0) ? finalSpeed : -finalSpeed;
				}
			}
		}
		
		// prevent paddle out of bounds
		const safetyBuffer = 0.5;
		const minZ = -fieldHeight / 2 + paddleDepth / 2 + safetyBuffer;
		const maxZ = fieldHeight / 2 - paddleDepth / 2 - safetyBuffer;
		
		if (paddle2.position.z < minZ) {
			paddle2.position.z = minZ;
			paddle2.isMoving = false;
		} else if (paddle2.position.z > maxZ) {
			paddle2.position.z = maxZ;
			paddle2.isMoving = false;
		}
	}

	function moveAIForPaddle1() {
		// initialize properties
		if (!paddle1.lastUpdateTime) {
			paddle1.lastUpdateTime = null;
			paddle1.targetPosition = 0;
			paddle1.previousTarget = 0;
			paddle1.currentTarget = 0;
			paddle1.interpolationStart = 0;
			paddle1.interpolationDuration = 300;
			paddle1.isMoving = false;
			paddle1.reactionDelay = 200;
		}

		const currentTime = Date.now();
		if (!paddle1.lastUpdateTime || currentTime - paddle1.lastUpdateTime > 1000) {
			// calculate target position
			paddle1.targetPosition = calculateTargetPosition(paddle1, false);
			
			const edgeBuffer = paddleDepth / 2 + 5;
			const topLimit = -fieldHeight / 2 + edgeBuffer;
			const bottomLimit = fieldHeight / 2 - edgeBuffer;
			
			paddle1.targetPosition = Math.max(topLimit, Math.min(bottomLimit, paddle1.targetPosition));
			
			paddle1.lastUpdateTime = currentTime;
			paddle1.reactionDelay = Math.random() * 80 + 50;
			
			paddle1.previousTarget = paddle1.currentTarget || paddle1.targetPosition;
			paddle1.currentTarget = paddle1.targetPosition;
			paddle1.interpolationStart = currentTime + paddle1.reactionDelay;
			paddle1.interpolationDuration = 300 + Math.random() * 250;
			
			paddle1.isMoving = true;
		}
		
		if (currentTime > paddle1.interpolationStart) {
			const elapsed = currentTime - paddle1.interpolationStart;
			const progress = Math.min(elapsed / paddle1.interpolationDuration, 1);
			
			// Smooth movement
			const easedProgress = progress < 0.5
				? 2 * progress * progress
				: 1 - Math.pow(-2 * progress + 2, 2) / 2;
			
			const interpolatedTarget = paddle1.previousTarget + 
				(paddle1.currentTarget - paddle1.previousTarget) * easedProgress;
			
			const distanceToTarget = interpolatedTarget - paddle1.position.z;
			const absDistance = Math.abs(distanceToTarget);
			
			const deadZone = 1.5;
			
			if (progress >= 1.0 && absDistance <= deadZone) {
				paddle1.isMoving = false;
			}
			
			if (paddle1.isMoving && absDistance > deadZone) {
				const baseSpeed = paddleSpeed * 0.9;
				
				const speedFactor = Math.min(1.0, absDistance / 15);
				const finalSpeed = baseSpeed * speedFactor;
				
				if (Math.random() < 0.05) {
					paddle1.position.z += (distanceToTarget > 0) ? -finalSpeed : finalSpeed;
				} else {
					paddle1.position.z += (distanceToTarget > 0) ? finalSpeed : -finalSpeed;
				}
			}
		}
		
		// Prevent paddle out of bounds
		const safetyBuffer = 0.5;
		const minZ = -fieldHeight / 2 + paddleDepth / 2 + safetyBuffer;
		const maxZ = fieldHeight / 2 - paddleDepth / 2 - safetyBuffer;
		
		if (paddle1.position.z < minZ) {
			paddle1.position.z = minZ;
			paddle1.isMoving = false;
		} else if (paddle1.position.z > maxZ) {
			paddle1.position.z = maxZ;
			paddle1.isMoving = false;
		}
	}


	function calculateTargetPosition(paddle, isRightPaddle) {
		// check if ball is moving toward paddle
		const isBallMovingTowardPaddle = isRightPaddle ? ballDirX > 0 : ballDirX < 0; // ballDirX is negative for left paddle
		
		if (isBallMovingTowardPaddle) {
			// calculate distance and time for ball to reach paddle
			const paddleX = paddle.position.x;
			const distanceToTravel = isRightPaddle ? 
				(paddleX - ball.position.x - RADIUS - paddleWidth/2) : 
				(ball.position.x - paddleX - RADIUS - paddleWidth/2);
				
			const ballXSpeed = Math.abs(ballDirX * ballSpeed);
			if (ballXSpeed < 0.1) return paddle.position.z;
			
			const timeToReachPaddle = distanceToTravel / ballXSpeed;
			
			// predict where ball will be at paddle x-position
			let predictedZ = ball.position.z + (ballDirZ * ballSpeed * timeToReachPaddle);
			
			// calculate bounces top and bot
			const topBoundary = -fieldHeight / 2 + RADIUS;
			const bottomBoundary = fieldHeight / 2 - RADIUS;
			

			if (Math.random() < 0.1) { // 10% chance of poor prediction
				predictedZ = ball.position.z + (ballDirZ * ballSpeed * timeToReachPaddle * 0.7);
			} else {
				// 90% chance of more accurate prediction with bounce calculation
				while (predictedZ < topBoundary || predictedZ > bottomBoundary) {
					if (predictedZ < topBoundary) {
						predictedZ = topBoundary + (topBoundary - predictedZ);
					} else if (predictedZ > bottomBoundary) {
						predictedZ = bottomBoundary - (predictedZ - bottomBoundary);
					}
				}
			}
			
			const difficultyError = Math.min(ballSpeed * 1.5, 10);
			predictedZ += (Math.random() * 2 - 1) * difficultyError * 0.3;
			
			if (Math.random() < 0.3) {
				const strategicOffset = (Math.random() - 0.5) * (paddleDepth * 0.6);
				return predictedZ + strategicOffset;
			}
			
			return predictedZ;
		} else {
			if (Math.random() < 0.15) {
				// Random movement occasionally
				return (Math.random() > 0.5 ? 1 : -1) * (fieldHeight/5);
			}
			return ball.position.z * 0.3 + ((Math.random() * fieldHeight/4) - fieldHeight/8);
		}
	}


  
	  function goalManagement() {
		if (ball.position.x >= paddle2.position.x + paddleWidth / 2) {
		  score1++;
		  updateScores();
		  resetBall();
		}
		if (ball.position.x <= paddle1.position.x - paddleWidth / 2) {
		  score2++;
		  updateScores();
		  resetBall();
		}
	  }
  
	  function updateScores() {
		score1Element.textContent = score1;
		score2Element.textContent = score2;
	  }
  
	  function resetBall() {
		ballSpeed = 0;
		ballDirZ = 0;
		ball.position.set(0, RADIUS, 0);
		paddle1.position.z = paddleDepth / 2;
		paddle2.position.z = paddleDepth / 2;
		setTimeout(() => {
		  ballDirX = (score1 > score2) ? -1 : 1;
		  ballDirZ = Math.random() * 2 - 1;
		  ballSpeed = 3;
		}, 2000);
	  }
  
	  function scoreCheck() {
		if (score1 >= winningScore) {
		  ballSpeed = 0;
		  isRunning = 1;
		} else if (score2 >= winningScore) {
		  ballSpeed = 0;
		  isRunning = 2;
		}
	  }
  
	  function resetGame() {
		score1 = 0;
		score2 = 0;
		updateScores();
		ball.position.set(0, RADIUS, 0);
		ballSpeed = 3;
		ballDirX = -1;
		ballDirZ = 1;
		paddle1.position.z = paddleDepth / 2;
		paddle2.position.z = paddleDepth / 2;
		isRunning = 0;
		winnerElement.innerText = '';
	  }
	  
  

	function update() {
		renderer.render(scene, camera);
		
		//game logic
		if (window.matchInProgress) { // check if game is still running
		ballMovement();
		if (!isAI1) playersMovement(); // check if player 1 is AI
		else moveAIForPaddle1();
		
		if (!isAI2) playersMovement(); // check if player 2 is AI
		else moveAIForPaddle2();
		
		paddleCollide();
		goalManagement();
		scoreCheck();
		}
	
		if (isRunning !== 0) {
		//game over
		window.matchInProgress = false;
		
		ballSpeed = 0;
		ball.position.set(0, RADIUS, 0);
		renderer.render(scene, camera);
		
		let winner;
		if (isRunning === 1) {
			winnerElement.innerText = `${playername1} Wins !`;
			winner = { name: playername1, type: type1 };
		} else if (isRunning === 2) {
			winnerElement.innerText = `${playername2} Wins !`;
			winner = { name: playername2, type: type2 };
		}
		
		// Show return button
		showReturnButton(() => {
			cleanup();
			resolve(winner);
		});
		
		return;
		}
		
		//store the id of the animation frame so we can cancel it later
		window.animationFrameId = requestAnimationFrame(update);
	}



	function showReturnButton(callback) {
		// remove any existing buttons
		const existingButtons = document.querySelectorAll("#returnToBracketBtn");
		existingButtons.forEach(btn => btn.remove());
		
		// create the button
		const returnBtn = document.createElement("button");
		returnBtn.textContent = "Retour au Bracket";
		returnBtn.id = "returnToBracketBtn";
		returnBtn.classList.add("btn", "btn-warning", "btn-lg");
		returnBtn.style.padding = "10px 20px"; // Larger padding
		returnBtn.style.fontSize = "1.2rem"; // Larger font
		returnBtn.style.margin = "20px auto"; // Center with margin
		returnBtn.style.display = "block"; // Full width block
		
		// add to the tournament container
		const tournamentContainer = document.getElementById("tournament");
		if (tournamentContainer) {
		tournamentContainer.appendChild(returnBtn);
		}
		

		returnBtn.addEventListener("click", function() {
		// prevent multiple clicks
		returnBtn.disabled = true;
		returnBtn.textContent = "Retour en cours...";
		
		// hide game elements
		const winnerElement = document.getElementById("winScreen");
		if (winnerElement) winnerElement.innerText = "";
		
		const gameContainer = document.getElementById("tournament");
		if (gameContainer) gameContainer.style.display = "none";
		
		// show the bracket and restore button
		const bracketContainer = document.getElementById("bracketContainer");
		if (bracketContainer) bracketContainer.style.display = "flex";
		
		const btnContainer = document.getElementById("bracketBtnContainer");
		if (btnContainer) btnContainer.style.display = "block";
		
		const nextMatchBtn = document.getElementById("nextMatchBtn");
		if (nextMatchBtn) {
			nextMatchBtn.disabled = false;
			nextMatchBtn.style.display = "inline-block";
			nextMatchBtn.textContent = "Lancer le prochain match";
		}
		
		// remove the button after returning to bracket
		returnBtn.remove();
		callback();
		}, { once: true }); 
	}
	  
	  // function to clean up event listeners and game elements
	  function cleanup() {
		document.removeEventListener('keydown', onKeyDown);
		document.removeEventListener('keyup', onKeyUp);
		
		// clear any remaining animations
		if (window.animationFrameId) {
		  cancelAnimationFrame(window.animationFrameId);
		}
	  }
	
	  function resetGame() {
		score1 = 0;
		score2 = 0;
		updateScores();
		ball.position.set(0, RADIUS, 0);
		ballSpeed = 3;
		ballDirX = -1;
		ballDirZ = 1;
		paddle1.position.z = paddleDepth / 2;
		paddle2.position.z = paddleDepth / 2;
		isRunning = 0;
		winnerElement.innerText = '';
	  }
	
	  resetGame();
	  startCountdown(() => {
		update();
	  });
	});
  }
  