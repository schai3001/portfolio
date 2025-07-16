document.addEventListener("profilEvent", function() { //event script stats
    const totalGamesElement = document.getElementById('totalGames');
    const totalWinsElement = document.getElementById('totalWins');
    const totalDefeatsElement = document.getElementById('totalDefeats');
    const winPercentageElement = document.getElementById('winPercentage');
    const gameHistoryBody = document.getElementById('gameHistoryBody');
    const paginationControls = document.getElementById('paginationControls');
    
    const gamesPerPage = 10; //number of games per page
    let currentPage = 1;
    let totalGames = 0;
    let allGames = [];
    
    // false data if error or no data
    const fallbackStats = {
        number_of_game: 0,
        number_of_win: 0,
        number_of_defeat: 0,
        win_percentage: 0
    };
    
    const fallbackGames = [];
    
    // load profile data
    async function loadProfileData() {
        try {
            const token = localStorage.getItem("access_token");
            if (!token) {
                throw new Error("Non connecté");
            }
            
            const response = await fetch("/api/profil/", {
                method: 'GET',
                headers: {
                    "Authorization": `Bearer ${token}`
                }
            });
            
            if (!response.ok) {
                throw new Error("Erreur lors du chargement des données du profil");
            }
            
            const responseData = await response.json();
            const data = responseData.data;
            
            // Afficher les statistiques
            displayUserStats({
                number_of_game: data.number_of_game || 0,
                number_of_win: data.number_of_win || 0,
                number_of_defeat: data.number_of_defeat || 0,
                win_percentage: data.win_percentage || 0
            });
            
            // Stocker toutes les parties et les afficher avec pagination
            allGames = data.games || [];
            totalGames = allGames.length;
            displayGameHistory(currentPage);
            
        } catch (error) {
            console.error('Erreur lors du chargement du profil:', error);
            displayUserStats(fallbackStats);
            displayGameHistory(1);
        }
    }
    
    // Fonction pour afficher les statistiques
    function displayUserStats(stats) {
        totalGamesElement.textContent = stats.number_of_game;
        totalWinsElement.textContent = stats.number_of_win;
        totalDefeatsElement.textContent = stats.number_of_defeat;
        winPercentageElement.textContent = `${stats.win_percentage}%`;
        
        createPerformanceChart(stats);
    }
    
    // Fonction pour afficher l'historique des parties avec pagination
    function displayGameHistory(page) {
        gameHistoryBody.innerHTML = '';
        
        if (allGames.length === 0) {
            gameHistoryBody.innerHTML = '<tr><td colspan="4" class="text-center">Aucune partie jouée pour le moment</td></tr>';
            return;
        }
        
        // Calculer les indices pour la pagination
        const startIndex = (page - 1) * gamesPerPage;
        const endIndex = Math.min(startIndex + gamesPerPage, allGames.length);
        const paginatedGames = allGames.slice(startIndex, endIndex);
        
        // Remplir le tableau avec les données
        paginatedGames.forEach(game => {
            const row = document.createElement('tr');
            
            if (game.win) {
                row.classList.add('table-success');
            } else {
                row.classList.add('table-danger');
            }
            
            let formattedDate = 'N/A';
            if (game.date) {
                const gameDate = new Date(game.date);
                formattedDate = gameDate.toLocaleDateString('fr-FR');
            }
            
            // Vérifier si c'est un tournoi
            if (game.opponent === 'Tournament') {
                row.innerHTML = `
                    <td>${formattedDate}</td>
                    <td>${game.tournament_players || ''}</td>
                    <td>Tournoi</td>
                    <td>${game.win ? 'Victoire' : 'Défaite'}</td>
                `;
            } else {
                row.innerHTML = `
                    <td>${formattedDate}</td>
                    <td>${game.opponent || 'Inconnu'}</td>
                    <td>${game.my_score || 0} - ${game.opponent_score || 0}</td>
                    <td>${game.win ? 'Victoire' : 'Défaite'}</td>
                `;
            }
            
            gameHistoryBody.appendChild(row);
        });
        
        updatePagination(page, Math.ceil(totalGames / gamesPerPage));
    }
    
    // Fonction pour pagination
    function updatePagination(currentPage, totalPages) {
        paginationControls.innerHTML = '';
        
        if (totalPages <= 1) return;
        
        const paginationNav = document.createElement('nav');
        paginationNav.setAttribute('aria-label', 'Navigation des pages');
        
        const ul = document.createElement('ul');
        ul.className = 'pagination';
        
        const prevLi = document.createElement('li');
        prevLi.className = `page-item ${currentPage === 1 ? 'disabled' : ''}`;
        
        const prevLink = document.createElement('a');
        prevLink.className = 'page-link';
        prevLink.href = '#';
        prevLink.textContent = 'Précédent';
        
        if (currentPage > 1) {
            prevLink.addEventListener('click', (e) => {
                e.preventDefault();
                displayGameHistory(currentPage - 1);
            });
        }
        
        prevLi.appendChild(prevLink);
        ul.appendChild(prevLi);
        
        for (let i = 1; i <= totalPages; i++) {
            const pageLi = document.createElement('li');
            pageLi.className = `page-item ${i === currentPage ? 'active' : ''}`;
            
            const pageLink = document.createElement('a');
            pageLink.className = 'page-link';
            pageLink.href = '#';
            pageLink.textContent = i;
            
            pageLink.addEventListener('click', (e) => {
                e.preventDefault();
                if (i !== currentPage) {
                    displayGameHistory(i);
                }
            });
            
            pageLi.appendChild(pageLink);
            ul.appendChild(pageLi);
        }
        
        const nextLi = document.createElement('li');
        nextLi.className = `page-item ${currentPage === totalPages ? 'disabled' : ''}`;
        
        const nextLink = document.createElement('a');
        nextLink.className = 'page-link';
        nextLink.href = '#';
        nextLink.textContent = 'Suivant';
        
        if (currentPage < totalPages) {
            nextLink.addEventListener('click', (e) => {
                e.preventDefault();
                displayGameHistory(currentPage + 1);
            });
        }
        
        nextLi.appendChild(nextLink);
        ul.appendChild(nextLi);
        
        paginationNav.appendChild(ul);
        paginationControls.appendChild(paginationNav);
    }
    
    // Fonction pour créer le graphique
    function createPerformanceChart(stats) {
        const chartContainer = document.getElementById('performanceChart');
        chartContainer.innerHTML = '';
        
        // Si aucune partie jouée, afficher un message
        if (stats.number_of_game === 0) {
            const noDataMessage = document.createElement('div');
            noDataMessage.style.textAlign = 'center';
            noDataMessage.style.padding = '50px 0';
            noDataMessage.style.color = '#666';
            noDataMessage.style.fontSize = '16px';
            noDataMessage.textContent = 'Aucune partie jouée pour le moment';
            
            chartContainer.appendChild(noDataMessage);
            return;
        }
        
        const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
        svg.setAttribute('width', '100%');
        svg.setAttribute('height', '250');
        svg.setAttribute('viewBox', '0 0 200 200');
        
        // Configuration du graphique
        const centerX = 100;
        const centerY = 100;
        const outerRadius = 80;
        const innerRadius = 40; // Pour créer efet donut
        
        // Calcul des valeurs pour le graphique
        const total = stats.number_of_game;
        const wins = stats.number_of_win;
        const defeats = stats.number_of_defeat;
        
        const winPercentage = total > 0 ? (wins / total) : 0;
        const defeatPercentage = total > 0 ? (defeats / total) : 0;
        
        // Couleurs
        const winColor = 'rgba(40, 167, 69, 0.7)';
        const winBorder = 'rgba(40, 167, 69, 1)';
        const defeatColor = 'rgba(220, 53, 69, 0.7)';
        const defeatBorder = 'rgba(220, 53, 69, 1)';
        
        // Création des segments
        if (total > 0) {
            // Segment des victoires
            if (wins > 0) {
                const winArc = createDonutSegment(
                    centerX, 
                    centerY, 
                    outerRadius, 
                    innerRadius, 
                    0, 
                    2 * Math.PI * winPercentage,
                    winColor,
                    winBorder
                );
                svg.appendChild(winArc);
            }
            
            // Segment des défaites
            if (defeats > 0) {
                const defeatArc = createDonutSegment(
                    centerX, 
                    centerY, 
                    outerRadius, 
                    innerRadius, 
                    2 * Math.PI * winPercentage, 
                    2 * Math.PI,
                    defeatColor,
                    defeatBorder
                );
                svg.appendChild(defeatArc);
            }
        }
        
        chartContainer.appendChild(svg);
        
        // Ajouter la légende
        const legend = document.createElement('div');
        legend.style.display = 'flex';
        legend.style.justifyContent = 'center';
        legend.style.marginTop = '20px';
        legend.style.gap = '20px';
        
        const winLegendItem = createLegendItem('Victoires', winColor, wins, total);
        const defeatLegendItem = createLegendItem('Défaites', defeatColor, defeats, total);
        
        legend.appendChild(winLegendItem);
        legend.appendChild(defeatLegendItem);
        
        chartContainer.appendChild(legend);
    }
    
    // Fonction pour créer un segment d'anneau (pour le graphique en anneau)
    function createDonutSegment(cx, cy, outerRadius, innerRadius, startAngle, endAngle, fillColor, strokeColor) {

        const startOuterX = cx + outerRadius * Math.cos(startAngle);
        const startOuterY = cy + outerRadius * Math.sin(startAngle);
        const endOuterX = cx + outerRadius * Math.cos(endAngle);
        const endOuterY = cy + outerRadius * Math.sin(endAngle);
        
        const startInnerX = cx + innerRadius * Math.cos(endAngle);
        const startInnerY = cy + innerRadius * Math.sin(endAngle);
        const endInnerX = cx + innerRadius * Math.cos(startAngle);
        const endInnerY = cy + innerRadius * Math.sin(startAngle);
        
        const largeArcFlag = endAngle - startAngle > Math.PI ? 1 : 0;
        
        // Créer le chemin SVG
        const path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
        
        const d = [
            `M ${startOuterX} ${startOuterY}`,
            `A ${outerRadius} ${outerRadius} 0 ${largeArcFlag} 1 ${endOuterX} ${endOuterY}`,
            `L ${startInnerX} ${startInnerY}`,
            `A ${innerRadius} ${innerRadius} 0 ${largeArcFlag} 0 ${endInnerX} ${endInnerY}`,
            'Z'
        ].join(' ');
        
        path.setAttribute('d', d);
        path.setAttribute('fill', fillColor);
        path.setAttribute('stroke', strokeColor);
        path.setAttribute('stroke-width', '1');
        
        // Ajouter un tooltip mosueover
        path.addEventListener('mouseover', function(e) {
            showTooltip(e, path, fillColor);
        });
        
        path.addEventListener('mouseout', function() {
            hideTooltip();
        });
        
        return path;
    }
    
    // Fonction pour créer un élément de légende
    function createLegendItem(label, color, value, total) {
        const item = document.createElement('div');
        item.style.display = 'flex';
        item.style.alignItems = 'center';
        
        const colorBox = document.createElement('div');
        colorBox.style.width = '15px';
        colorBox.style.height = '15px';
        colorBox.style.backgroundColor = color;
        colorBox.style.marginRight = '5px';
        
        const text = document.createElement('span');
        const percentage = total > 0 ? Math.round((value / total) * 100) : 0;
        text.textContent = `${label}: ${value} (${percentage}%)`;
        
        item.appendChild(colorBox);
        item.appendChild(text);
        
        return item;
    }
    
    // Fonctions pour le tooltip
    function showTooltip(event, element, color) {
        let tooltip = document.getElementById('chart-tooltip');
        
        if (!tooltip) {
            tooltip = document.createElement('div');
            tooltip.id = 'chart-tooltip';
            tooltip.style.position = 'absolute';
            tooltip.style.backgroundColor = 'rgba(0, 0, 0, 0.8)';
            tooltip.style.color = 'white';
            tooltip.style.padding = '5px 10px';
            tooltip.style.borderRadius = '3px';
            tooltip.style.fontSize = '14px';
            tooltip.style.pointerEvents = 'none';
            tooltip.style.zIndex = '1000';
            document.body.appendChild(tooltip);
        }
        
        // Déterminer le contenu et la position du tooltip
        tooltip.textContent = element.getAttribute('data-tooltip') || 'Information';
        tooltip.style.display = 'block';
        tooltip.style.left = `${event.pageX + 10}px`;
        tooltip.style.top = `${event.pageY + 10}px`;
    }
    
    function hideTooltip() {
        const tooltip = document.getElementById('chart-tooltip');
        if (tooltip) {
            tooltip.style.display = 'none';
        }
    }
    
    // Fonction pour récupérer un cookie
    function getCookie(name) {
        let cookieValue = null;
        if (document.cookie && document.cookie !== '') {
            const cookies = document.cookie.split(';');
            for (let i = 0; i < cookies.length; i++) {
                const cookie = cookies[i].trim();
                if (cookie.substring(0, name.length + 1) === (name + '=')) {
                    cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                    break;
                }
            }
        }
        return cookieValue;
    }
    
    // Charger les données du profil au déclenchement de l'événement
    loadProfileData();
    
    // Fonction pour recharger les données
    function reloadData() {
        loadProfileData();
    }
    
    // Exposer les fonctions pour un accès externe
    window.statsManager = {
        reloadStats: reloadData,
        reloadHistory: reloadData
    };
});