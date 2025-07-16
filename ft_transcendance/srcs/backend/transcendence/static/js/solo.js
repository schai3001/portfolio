console.log("Mode solo chargé !");



startGameSolo();



function startGameSolo() {

    document.getElementById("game").style.display = "block";
    document.getElementById("loadingMessage").style.display = "none";
    document.getElementById("soloBtn").style.display = "none";
    document.getElementById("multiBtn").style.display = "none";


	// ------------------------------- //
	// ---------- COUNTDOWN ---------- //
	// ------------------------------- //
	
	// Setting up the scene, camera, lights, etc.
	
	// Define everything needed.

	// Get html/css elements
	const score1Element = document.getElementById('score1');
	const score2Element = document.getElementById('score2');
	const player1Element = document.getElementById('playerA');
	const player2Element = document.getElementById('playerB');
	const winnerElement = document.getElementById('winScreen');
	const countdownElement = document.getElementById('countdown');
	let turn = localStorage.getItem('turn');
	if (!turn) {
		turn = 1;
		localStorage.setItem('turn', turn);
	}
	else {
		turn++;
		localStorage.setItem('turn', turn);
	}

	// Set playground
	const WIDTH = 1000;
	const HEIGHT = 650;
	const fieldWidth = 400, fieldHeight = 200;
	
	// Set some camera attributes.
	const VIEW_ANGLE = 45;
	const ASPECT = WIDTH / HEIGHT;
	const NEAR = 0.1;
	const FAR = 10000;
	let renderer, scene, camera, pointLightOne, pointLightTwo;

	// Set up the sphere vars
	const RADIUS = 5;
	const SEGMENTS = 16;
	const RINGS = 16;

	// Set up border vars
	const barrierThickness = 2;
	const barrierHeight = 5;
	const barrierDepth = fieldHeight;

	// Set elements of the game
	let ball, paddle1, paddle2;
	const paddleWidth = 10, paddleHeight = 10;
	const paddleDepth = 50, paddleQuality = 1;
	let paddle1DirZ = 0, paddle2DirZ = 0, paddleSpeed = 3;
	let ballDirX = -1, ballDirZ = 1, ballSpeed = 3;

	//score management
	let score1 = 0;
	let score2 = 0;
	let player1 = "Player";
	let player2 = "CPU";
	let winner = 7;

	// Movement management
	var KEYDOWN = "KeyS", KEYUP = "KeyW", 
	KEYDOWNTWO = "ArrowDown", KEYUPTWO = "ArrowUp";

	var goUpOne = false, goDownOne = false, 
	goUpTwo = false, goDownTwo = false;

	document.addEventListener('keydown', onKeyDown);
	document.addEventListener('keyup', onKeyUp);
	
	//stop the game
	var isRunning = 0;

	player1Element.textContent = player1;
	player2Element.textContent = player2;
	
	// Function to start the countdown
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
				callback(); // Start the game after the countdown
			}
		}, 1000);
	}
	
	// ------------------------------- //
	// ----------- DISPLAY ----------- //
	// ------------------------------- //
	
	// Setting up : Scene, Camera, Spotlights. 
	
	// /\ -- CAMERA -- /\
	
	// Create a WebGL renderer, camera, and a scene
	renderer = new THREE.WebGLRenderer({alpha: true});
	
	camera = new THREE.PerspectiveCamera(
		VIEW_ANGLE,
		ASPECT,
		NEAR,
		FAR
	);
	
	scene = new THREE.Scene();
	
	// Add the camera to the scene.
	scene.add(camera);
	
	camera.position.set(0, 300, 350);
	camera.lookAt(scene.position);
	
	// Start the renderer.
	renderer.setSize(WIDTH, HEIGHT);
	
	// Get the DOM element to attach to
	const container = document.getElementById('gameCanvas');
	
	// Attach the renderer-supplied DOM element.
	container.appendChild(renderer.domElement);
	
	
	// -------------------------------------------
	
	
	// /\ -- LIGHTS -- /\
	
	// create a first point light for red player
	pointLightOne = new THREE.PointLight(0xF8D898);
	
	// set its position
	pointLightOne.position.set(100, 100, 0);
	pointLightOne.intensity = 1;
	pointLightOne.distance = 0;
	
	// add to the scene
	scene.add(pointLightOne);
	
	// create a second point light for blue player
	pointLightTwo = new THREE.PointLight(0xF8D898);
	
	// set its position
	pointLightTwo.position.set(-100, 100, 0);
	pointLightTwo.intensity = 1;
	pointLightTwo.distance = 0;
	
	// add to the scene
	scene.add(pointLightTwo);
	
	
	// -------------------------------------------
	

	// -------------------------------- //
	// -------- GAME MATERIALS -------- //
	// -------------------------------- //
	
	// Setting up : Ball, Ground, GroundBorder, Paddles
	
	
	// /\ -- Ball -- /\
	
	// ball's x-direction, y-direction and speed per frame
	//create ball material
	const sphereMaterial = new THREE.MeshLambertMaterial({ color: 0x303030 });
	
	// Create a new mesh with sphere geometry
	ball = new THREE.Mesh(
		new THREE.SphereGeometry(RADIUS, SEGMENTS, RINGS),
		sphereMaterial
	);
	
	// Position the sphere on the field
	ball.position.set(0, RADIUS, 0); // Adjusted position
	
	// Finally, add the sphere to the scene.
	scene.add(ball);
	
	
	// -------------------------------------------
	
	
	// /\ -- GROUND -- /\
	
	// field variables
	
	// set up the playing surface plane 
	const planeWidth = fieldWidth, planeHeight = fieldHeight, planeQuality = 32;
	
	//create plane material
	const planeMaterial = new THREE.MeshLambertMaterial({ color: 0xFFD5A1 });
	
	// create the playing surface plane
	const plane = new THREE.Mesh(
		new THREE.PlaneGeometry(planeWidth, planeHeight, planeQuality, planeQuality),
		planeMaterial
	);
	
	// Rotate the plane to be flat horizontally
	plane.rotation.x = -Math.PI / 2;
	
	// Move the plane down so it doesn't intersect the camera view
	plane.position.y = -5;
	
	scene.add(plane);
	
	
	// -------------------------------------------
	
	
	// /\ -- GROUND BORDER -- /\

	//create borders material
	const barrierMaterial = new THREE.MeshLambertMaterial({ color: 0x444444 });

	// farthest border
	// Barrière du haut
	const topBarrier = new THREE.Mesh(
		new THREE.BoxGeometry(fieldWidth, barrierHeight, barrierThickness),
		barrierMaterial
	);

	topBarrier.position.set(0, barrierHeight / 2, -fieldHeight / 2 - barrierThickness / 2);

	scene.add(topBarrier);

	// closest border
	const bottomBarrier = new THREE.Mesh(
		new THREE.BoxGeometry(fieldWidth, barrierHeight, barrierThickness),
		barrierMaterial
	);

	bottomBarrier.position.set(0, barrierHeight / 2, fieldHeight / 2 + barrierThickness / 2);

	scene.add(bottomBarrier);


	// -------------------------------------------


	// /\ -- PADDLES -- /\

	//create paddles material
	const paddle1Material = new THREE.MeshLambertMaterial({ color: 0x0000FF });
	const paddle2Material = new THREE.MeshLambertMaterial({ color: 0xFF0000 });

	// set up paddle 1
	paddle1 = new THREE.Mesh(
		new THREE.BoxGeometry(
			paddleWidth,
			paddleHeight,
			paddleDepth,
			paddleQuality,
			paddleQuality,
			paddleQuality),
			paddle1Material);
	
	// add the paddle to the scene
	scene.add(paddle1);


	// Set up the second paddle
	paddle2 = new THREE.Mesh(
		new THREE.BoxGeometry(
			paddleWidth,
			paddleHeight,
			paddleDepth,
			paddleQuality,
			paddleQuality,
			paddleQuality),
			paddle2Material);

	// Add the second paddle to the scene
	scene.add(paddle2);

	// set paddles on each side of the table
	paddle1.position.x = -fieldWidth / 2 + paddleWidth;
	paddle2.position.x = fieldWidth / 2 - paddleWidth;

	// lift paddles over playing surface
	paddle1.position.z = paddleDepth / 2;
	paddle2.position.z = paddleDepth / 2;


	// -------------------------------------------


	// -------------------------------- //
	// -------- GAME FUNCTIONS -------- //
	// -------------------------------- //

	// /\ -- Ball movement n rebound -- /\

	function ballMovement() {

		//X = left / right
		//Y = up / down (not usefull)
		//Z = farther / closer

		// update ball position over time
		ball.position.x += ballDirX * ballSpeed;
		ball.position.z += ballDirZ * ballSpeed;

		// if ball hits the top or bottom barrier
		// if (ball.position.z <= -fieldHeight / 2 + RADIUS || ball.position.z >= fieldHeight / 2 - RADIUS) {
		// 	ballDirZ = -ballDirZ;
		// }

		if (ball.position.z <= -fieldHeight / 2 + RADIUS) {
			ball.position.z = -fieldHeight / 2 + RADIUS + 1; // Push it back slightly
			ballDirZ = Math.abs(ballDirZ); // Ensure it moves away
		}
		if (ball.position.z >= fieldHeight / 2 - RADIUS) {
			ball.position.z = fieldHeight / 2 - RADIUS - 1; // Push it back slightly
			ballDirZ = -Math.abs(ballDirZ); // Ensure it moves away
		}

		// limit ball's z-speed to 2x the x-speed
		if (ballDirZ > ballSpeed * 2) {
			ballDirZ = ballSpeed * 2;
		} else if (ballDirZ < -ballSpeed * 2) {
			ballDirZ = -ballSpeed * 2;
		}
	}

	function paddelCollide() {
		// Paddle1 (left)
		if (ball.position.x - RADIUS <= paddle1.position.x + paddleWidth / 2 
			&& ball.position.x + RADIUS >= paddle1.position.x - paddleWidth / 2) {
			if (ball.position.z <= paddle1.position.z + paddleDepth / 2 
				&& ball.position.z >= paddle1.position.z - paddleDepth / 2) {
				if (ballDirX < 0) {
					ballDirX = -ballDirX;
					// Calculer la nouvelle direction Z
					let impactPoint = (ball.position.z - paddle1.position.z) / (paddleDepth / 2);
					ballDirZ = impactPoint * ballSpeed / 1.5;
					ballSpeed += 0.2;
					if (Math.abs(ballDirZ) < 0.5) { 
						ballDirZ = ballDirZ > 0 ? 0.5 : -0.5;
					}
				}
			}
		}
	
		// Paddle2 (right)
		if (ball.position.x + RADIUS >= paddle2.position.x - paddleWidth / 2 
			&& ball.position.x - RADIUS <= paddle2.position.x + paddleWidth / 2) {
			if (ball.position.z <= paddle2.position.z + paddleDepth / 2 
				&& ball.position.z >= paddle2.position.z - paddleDepth / 2) {
				if (ballDirX > 0) {
					ballDirX = -ballDirX;
					// Calculer la nouvelle direction Z
					let impactPoint = (ball.position.z - paddle2.position.z) / (paddleDepth / 2);
					ballDirZ = impactPoint * ballSpeed / 1.5;
					ballSpeed += 0.2;
					if (Math.abs(ballDirZ) < 0.5) { 
						ballDirZ = ballDirZ > 0 ? 0.5 : -0.5;
					}
				}
			}
		}
	}
	

	// -------------------------------------------


	// /\ -- Paddle Management -- /\

	//the comments in moving functions are for multiPlayers mode
	//bot function will control paddle2 in training mode

	function onKeyDown(event) {

		if (event.code == KEYDOWN) {
			goDownOne = true;
		}
		if (event.code == KEYUP) {
			goUpOne = true;
		} 

		// if (event.code == KEYDOWNTWO) {
		// 	goDownTwo = true;
		// }
		// if (event.code == KEYUPTWO) {
		// 	goUpTwo = true;
		// } 
	}

	function onKeyUp(event) {

		if (event.code == KEYDOWN) {
			goDownOne = false;
		}
		if (event.code == KEYUP) {
			goUpOne = false;
		} 

		// if (event.code == KEYDOWNTWO) {
		// 	goDownTwo = false;
		// }
		// if (event.code == KEYUPTWO) {
		// 	goUpTwo = false;
		// } 
	}

	function playersMovement() {

		if (goUpOne && paddle1.position.z > -fieldHeight / 2 + paddleDepth / 2 + 2) {
			paddle1.position.z -= paddleSpeed;
		}
		if (goDownOne && paddle1.position.z < fieldHeight / 2 - paddleDepth / 2 - 2) {
			paddle1.position.z += paddleSpeed;
		}

		// if (goUpTwo && paddle2.position.z > -fieldHeight / 2 + paddleDepth / 2 + 2) {
		// 	paddle2.position.z -= paddleSpeed;
		// }
		// if (goDownTwo && paddle2.position.z < fieldHeight / 2 - paddleDepth / 2 - 2) {
		// 	paddle2.position.z += paddleSpeed;
		// }

	}



	paddle2.lastUpdateTime = null; // Last update time for AI
	paddle2.targetPosition = 0; // Target position for AI
	paddle2.previousTarget = 0;
	paddle2.currentTarget = 0;
	paddle2.interpolationStart = 0; // Interpolation start time
	paddle2.interpolationDuration = 300; // Interpolation duration
	paddle2.isMoving = false; // Movement flag
	paddle2.reactionDelay = 200; // Reaction delay



	function moveAI() {

		//decision making update every 1.5 seconds

		const currentTime = Date.now(); // get current time in milliseconds
		if (!paddle2.lastUpdateTime || currentTime - paddle2.lastUpdateTime > 1000) { // update every 1.5 seconds
			paddle2.targetPosition = calculateTargetPosition(); // calculate new target position
			

			const edgeBuffer = paddleDepth / 2 + 5; // buffer to avoid jittering at edges
			const topLimit = -fieldHeight / 2 + edgeBuffer; // top limit
			const bottomLimit = fieldHeight / 2 - edgeBuffer; // bottom limit
			

			paddle2.targetPosition = Math.max(topLimit, Math.min(bottomLimit, paddle2.targetPosition));
			
			paddle2.lastUpdateTime = currentTime; 
			paddle2.reactionDelay = Math.random() * 80 + 50; // max 130ms reaction time
			
			paddle2.previousTarget = paddle2.currentTarget || paddle2.targetPosition; // store previous target
			paddle2.currentTarget = paddle2.targetPosition; // update current target
			paddle2.interpolationStart = currentTime + paddle2.reactionDelay; // set interpolation start time
			paddle2.interpolationDuration = 300 + Math.random() * 250; // 300-550ms smooth transition
			
			// set a movement flag to prevent starting and stopping
			paddle2.isMoving = true;
		}
		


		if (currentTime > paddle2.interpolationStart) { // start interpolation
			// calculate elapsed time since interpolation start

			const elapsed = currentTime - paddle2.interpolationStart; // elapsed time
			const progress = Math.min(elapsed / paddle2.interpolationDuration, 1); // calculate progress (0 to 1)
			
			// apply easing for smooth movement
			// divide the movement in 2 parts
			// 1st part is acceleration to midpoint
			// 2nd part is deceleration to target
			const easedProgress = progress < 0.5
				? 2 * progress * progress
				: 1 - Math.pow(-2 * progress + 2, 2) / 2;
			
			// calculate current interpolated target
			const interpolatedTarget = paddle2.previousTarget + 
				(paddle2.currentTarget - paddle2.previousTarget) * easedProgress;
			
			// calculate distance to target
			const distanceToTarget = interpolatedTarget - paddle2.position.z;
			const absDistance = Math.abs(distanceToTarget);
			
			// deadzone to stop movement, fix jittering
			const deadZone = 1.5;
			
			// stop moving when movement is complete
			if (progress >= 1.0 && absDistance <= deadZone) {
				paddle2.isMoving = false;
			}
			
			if (paddle2.isMoving && absDistance > deadZone) {
				const baseSpeed = paddleSpeed
				
				const speedFactor = Math.min(1.0, absDistance / 15);
				const finalSpeed = baseSpeed * speedFactor;
				
				if (Math.random() < 0.05) { // 5% chance of mistake
					paddle2.position.z += (distanceToTarget > 0) ? -finalSpeed : finalSpeed;
				} else {
					paddle2.position.z += (distanceToTarget > 0) ? finalSpeed : -finalSpeed;
				}
			}
		}
		
		//fix jittering at edges
		const safetyBuffer = 0.5;
		const minZ = -fieldHeight / 2 + paddleDepth / 2 + safetyBuffer;
		const maxZ = fieldHeight / 2 - paddleDepth / 2 - safetyBuffer;
		
		if (paddle2.position.z < minZ) {
			paddle2.position.z = minZ;
			paddle2.isMoving = false; // stop movement when hitting boundary
		} else if (paddle2.position.z > maxZ) {
			paddle2.position.z = maxZ;
			paddle2.isMoving = false; // stop movement when hitting boundary
		}
	}
	
	function calculateTargetPosition() {

		if (ballDirX > 0) { // Only calculate when ball is moving toward AI
			// Calculate distance to travel to paddle
			const distanceToTravel = paddle2.position.x - ball.position.x - RADIUS - paddleWidth/2;
			// Calculate time to reach paddle
			const timeToReachPaddle = distanceToTravel / (ballDirX * ballSpeed);
			
			// Predict where ball will be when it reaches paddle's x-position
			let predictedZ = ball.position.z + (ballDirZ * ballSpeed * timeToReachPaddle);
			
			// Calculate bounces off top/bottom walls
			const topBoundary = -fieldHeight / 2 + RADIUS;
			const bottomBoundary = fieldHeight / 2 - RADIUS;
			
			
			if (Math.random() < 0.05) { // 20% chance of poor prediction

				predictedZ = ball.position.z + (ballDirZ * ballSpeed * timeToReachPaddle * 0.7); // Poor prediction with 30% diff
			} else {
				//80% chance of good prediction
				while (predictedZ < topBoundary || predictedZ > bottomBoundary) {  // Bounce off
					if (predictedZ < topBoundary) {
						predictedZ = topBoundary + (topBoundary - predictedZ); // Reflect off top
					} else if (predictedZ > bottomBoundary) {
						predictedZ = bottomBoundary - (predictedZ - bottomBoundary); // Reflect off bottom
					}
				}
			}
			

			const difficultyError = Math.min(ballSpeed * 1.5, 10); // Maximum error based on ball speed
			predictedZ += (Math.random() * 2 - 1) * difficultyError * 0.5; // Add random error
			

			if (Math.random() < 0.4) { // 40% chance of strategic offset
				const strategicOffset = (Math.random() - 0.5) * (paddleDepth * 0.8); // 80% of paddle depth
				return predictedZ + strategicOffset; // Apply strategic offset
			}
			
			return predictedZ;
		} else {
			// ball is moving away from AI, center paddle

			if (Math.random() < 0.15) { // 15% chance random movement
				return (Math.random() > 0.5 ? 1 : -1) * (fieldHeight/5); 
			}
			// 85% chance of centering paddle
			return ball.position.z * 0.2 + ((Math.random() * fieldHeight/4) - fieldHeight/8);
		}
	}

	
	

	// -------------------------------------------


	// /\ -- Score Management -- /\

	function goalManagement() {

		if (ball.position.x >= paddle2.position.x + paddleWidth / 2)
		{
			score1 += 1;
			updateScores();
			resetBall();
		}

		if (ball.position.x <= paddle1.position.x - paddleWidth / 2)
		{
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

		// Reset paddle positions
        paddle1.position.z = paddleDepth / 2;
        paddle2.position.z = paddleDepth / 2;
        paddle1DirZ = 0;
        paddle2DirZ = 0;


		paddle2.lastUpdateTime = null; // Last update time for AI
		paddle2.isMoving = false; // Movement flag


		setTimeout(() => {
			ballDirX = (score1 > score2) ? -1 : 1;
			ballDirZ = Math.random() * 2 - 1;
			ballSpeed = 3;
		}, 2000); 
	}

	function scoreCheck() {

		if (score1 >= winner)
		{
			ballSpeed = 0;
			isRunning = 1;

		}
		else if (score2 >= winner)
		{
			ballSpeed = 0;
			isRunning = 2;
		}
	}


	// reload the game page at the end of the game
	function reloadGamePage() {
		// remove event listeners
		document.removeEventListener('keydown', onKeyDown);
		document.removeEventListener('keyup', onKeyUp);
		
		// load the game page
		if (typeof loadPage === 'function') {
			loadPage('game');
		} else {
			location.hash = "game";
			window.dispatchEvent(new Event('hashchange'));
		}
	}

	function update() {
		// Draw!
		renderer.render(scene, camera);
		if (isRunning == 3)
		{
			return;
		}
		
		ballMovement();
		playersMovement();
		moveAI();
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
				winnerElement.textContent = `Player Wins !`;
				win = true;
			}
			else if (isRunning == 2) {
				winnerElement.textContent = `CPU Wins !`;
				win = false;
			}

			let data = {
				'opponent': 'CPU',
				'win': win,
				'my_score': score1,
				'opponent_score': score2,
			};
			
			// Function to get CSRF token
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
					// Get the access token from localStorage (not jwtToken)
					let accessToken = localStorage.getItem('access_token');
					let csrfToken = getCsrfToken();
					
					if (!accessToken) {
						console.error('No access token found');
						return;
					}
					
					const response = await fetch('/api/game_end/', {
						method: 'POST',
						headers: {
							'Authorization': `Bearer ${accessToken}`,
							'X-CSRFToken': csrfToken,
							'Content-Type': 'application/json',
						},
						body: JSON.stringify(data),
					});

					const respdata = await response.json();
					if (!response.ok) {
						console.error('Error from server:', respdata.error);
					} else {
						console.log('Game stats saved successfully:', respdata.message);
					}
				} catch (error) {
					console.error('Error during stats submission:', error);
				} finally {
					// reload the game page after 5 second
					console.log('reloading page game');
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

	// Start the countdown and then start the game
	startCountdown(() => {
		update();
	});
}
