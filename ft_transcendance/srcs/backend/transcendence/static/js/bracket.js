document.addEventListener("bracketEvent", function(){
  const players = window.tournamentPlayers;

  // first round
  const round1 = buildBracketRound(players);
  let bracketState = {
    rounds: [round1],
    currentRoundIndex: 0,
    currentMatchIndex: 0,
    isComplete: false,
    finalWinner: null,
    resultsSubmitted: false
  };

  renderBracket(bracketState);
  createNextMatchButton(bracketState);
});

// shuffle function (Fisher-Yates)
function shuffle(array) {
for (let i = array.length - 1; i > 0; i--) {
  const j = Math.floor(Math.random() * (i + 1));
  [array[i], array[j]] = [array[j], array[i]];
}
}

// bracket builder
function buildBracketRound(players) {
let matches = [];
shuffle(players); 

for (let i = 0; i < players.length; i += 2) {
  if (i + 1 < players.length) {
    // 2 players
    const p1Name = (typeof players[i] === "object") ? players[i].name : players[i];
    const p1Type = (typeof players[i] === "object") ? players[i].type : "user";
    const p2Name = (typeof players[i+1] === "object") ? players[i+1].name : players[i+1];
    const p2Type = (typeof players[i+1] === "object") ? players[i+1].type : "user";
    matches.push({
      player1: { name: p1Name, type: p1Type },
      player2: { name: p2Name, type: p2Type },
      winner: null
    });
  } else {
    // Bye : solo player
    const p1Name = (typeof players[i] === "object") ? players[i].name : players[i];
    const p1Type = (typeof players[i] === "object") ? players[i].type : "user";
    matches.push({
      player1: { name: p1Name, type: p1Type },
      player2: null,
      winner: p1Name // automatically advance to next round
    });
  }
}
return matches;
}

function renderBracket(bracketState) {
let container = document.getElementById("bracketContainer");
if(!container) {
  container = document.createElement("div");
  container.id = "bracketContainer";
  container.style.marginTop = "1rem";
  document.body.appendChild(container);
}
container.style.display = "flex";
container.innerHTML = "";

if (bracketState.isComplete) {

  if (!bracketState.resultsSubmitted && bracketState.finalWinner) {
    bracketState.resultsSubmitted = true; //avoid duplicate submission
    submitTournamentResults(bracketState.finalWinner, window.tournamentPlayers);
  }

  // display winner
const winnerSection = document.getElementById("winnerSection");
const winnerNameElement = document.getElementById("winnerName");
const winnerImageElement = document.getElementById("winnerImage");

winnerNameElement.textContent = `Vainqueur : \n ${bracketState.finalWinner.name}`;
winnerSection.style.display = "block";

const finalPic = bracketState.finalWinner.name === localStorage.getItem("currentUsername")
  ? localStorage.getItem("currentProfilePicture")
  : "";

if (finalPic) {
  winnerImageElement.src = finalPic;
  winnerImageElement.style.display = "block";
} else {
  winnerImageElement.style.display = "none";
}
  
  // Update tournament state
  if (window.tournamentState) {
    window.tournamentState.inProgress = false;
  }
  
  return;
}

// display round
bracketState.rounds.forEach((round, rIndex) => {
  const roundDiv = document.createElement("div");
  roundDiv.className = "round-column";
  roundDiv.style.marginRight = "2rem";
  roundDiv.innerHTML = `<h4>Round ${rIndex + 1}</h4>`;
  round.forEach((match) => {
    const matchDiv = document.createElement("div");
    matchDiv.className = "match";
    const p1 = match.player1?.name || "???";
    const p2 = match.player2?.name || "???";
  const winnerName = match.winner?.name || null;

  const trophy = " 🏆";
const winnerText = winnerName === p1 ? p1 + trophy : p1;
const loserText = winnerName === p2 ? p2 + trophy : p2;
  
  matchDiv.innerHTML = `
  <span class="${winnerName === p1 ? 'winner' : ''}">${winnerText}</span>
  <span>VS</span>
  <span class="${winnerName === p2 ? 'winner' : ''}">${loserText}</span>
`;
    roundDiv.appendChild(matchDiv);
  });
  container.appendChild(roundDiv);
});
}



function createNextMatchButton(bracketState) {
  // remove any existing button containers
  const existingBtnContainers = document.querySelectorAll("#bracketBtnContainer");
  existingBtnContainers.forEach(container => container.remove());
  
  // create a new button container 
  let btnContainer = document.createElement("div");
  btnContainer.id = "bracketBtnContainer";
  btnContainer.style.marginTop = "2rem";
  btnContainer.style.textAlign = "center";
  btnContainer.style.width = "100%"; 
  btnContainer.style.clear = "both"; 
  
  // create the button
  const nextMatchBtn = document.createElement("button");
  nextMatchBtn.id = "nextMatchBtn";
  nextMatchBtn.className = "btn btn-success btn-lg"; 
  nextMatchBtn.textContent = "Lancer le prochain match";
  nextMatchBtn.style.padding = "10px 20px"; 
  nextMatchBtn.style.fontSize = "1.2rem"; 
  
  // add the button to its container
  btnContainer.appendChild(nextMatchBtn);
  
  // get the bracket container and insert the button container after it
  const bracketContainer = document.getElementById("bracketContainer");
  if (!bracketContainer) {
    console.error("Bracket container not found, cannot add next match button");
    return;
  }
  
  // add the button container after the bracket
  bracketContainer.parentNode.insertBefore(btnContainer, bracketContainer.nextSibling);
  

  btnContainer.style.display = "block";
  nextMatchBtn.style.display = "inline-block";
  

  console.log("Next match button created and inserted after bracket");
  
  // set up the click handler
  nextMatchBtn.addEventListener("click", function() {
    if (bracketState.isComplete) {
      alert("Tournoi terminé !");
      nextMatchBtn.style.display = "none";
      return;
    }
    
    //prevent double-click
    nextMatchBtn.disabled = true;
    nextMatchBtn.textContent = "Match en cours...";
    
    startNextMatch(bracketState).then(() => {
      const currentRound = bracketState.rounds[bracketState.currentRoundIndex];
      if (bracketState.currentMatchIndex >= currentRound.length) {
        const winners = currentRound.map(match => match.winner);
        if (winners.length > 1) {
          const nextRound = buildBracketRound(winners);
          bracketState.rounds.push(nextRound);
          bracketState.currentRoundIndex++;
          bracketState.currentMatchIndex = 0;
        } else {
          bracketState.isComplete = true;
          bracketState.finalWinner = winners[0];
        }
      }
      
      renderBracket(bracketState);
      
      if (!bracketState.isComplete) {
        // only re-enable the button if the tournament is not complete
        nextMatchBtn.disabled = false;
        nextMatchBtn.textContent = "Lancer le prochain match";
        
        // make sure the button container is visible
        btnContainer.style.display = "block";
        nextMatchBtn.style.display = "inline-block";
        
        console.log("Next match button re-enabled after match");
      } else {
        // hide the button container if tournament is complete
        btnContainer.style.display = "none";
      }
      
      document.getElementById("tournamentDescription").style.display = "none";
    });
  });
}




function startNextMatch(bracketState) {
  return new Promise((resolve) => {
    const currentRound = bracketState.rounds[bracketState.currentRoundIndex];
    const match = currentRound[bracketState.currentMatchIndex];
    
    // if bye => no match
    if (!match.player2) {
      match.winner = match.player1;
      bracketState.currentMatchIndex++;
      resolve();
      return;
    }
    
    const p1 = match.player1.name;
    const t1 = match.player1.type;
    const p2 = match.player2.name;
    const t2 = match.player2.type;
    
    // hide bracket container and show game
    document.getElementById("bracketContainer").style.display = "none";
    document.getElementById("tournament").style.display = "block";

    // hide the button container and disable the button
    const btnContainer = document.getElementById("bracketBtnContainer");
    const nextMatchBtn = document.getElementById("nextMatchBtn");
    
    if (btnContainer) btnContainer.style.display = "none";
    if (nextMatchBtn) {
      nextMatchBtn.disabled = true;
      nextMatchBtn.textContent = "Match en cours...";
    }
    
    // global flag to indicate match is in progress
    window.matchInProgress = true;
    
    startGameTournament(p1, p2, t1, t2).then((winnerData) => {
      // update match result with the winner
      match.winner = winnerData;
      
      
      document.getElementById("tournament").style.display = "none";
      document.getElementById("bracketContainer").style.display = "flex";
      
      
      if (btnContainer) btnContainer.style.display = "block";
      if (nextMatchBtn) {
        nextMatchBtn.disabled = false;
        nextMatchBtn.textContent = "Lancer le prochain match";
        nextMatchBtn.style.display = "inline-block";
      }
      
      
      console.log("After match completion - is button visible:", 
                 nextMatchBtn ? nextMatchBtn.style.display !== "none" : "no button");
      
      bracketState.currentMatchIndex++;
      
      resolve();
    }).catch(error => {
      console.error("Error in match:", error);
      
      // reset the button state
      if (btnContainer) btnContainer.style.display = "block";
      if (nextMatchBtn) {
        nextMatchBtn.disabled = false;
        nextMatchBtn.textContent = "Lancer le prochain match";
        nextMatchBtn.style.display = "inline-block";
      }
      
      resolve();
    });
  });
}