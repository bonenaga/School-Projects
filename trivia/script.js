// Multiple Choice Buttons Logic
document.querySelectorAll('.mc-btn').forEach(button => {
    button.onclick = () => {
        const correct = button.getAttribute('data-correct') === 'true';
        document.getElementById('mc-feedback').textContent = correct ? 'Correct!' : 'Incorrect';
        button.style.backgroundColor = correct ? 'green' : 'red';
        // Reset other buttons
        document.querySelectorAll('.mc-btn').forEach(btn => {
            if (btn !== button) btn.style.backgroundColor = 'lightgray';
        });
    };
});

// Free Response Logic
function checkFreeResponse() {
    const answerField = document.getElementById('fr-answer');
    const answer = answerField.value.toLowerCase().trim();
    const correctAnswer = "mraw"; // Set the correct answer
    const isCorrect = answer === correctAnswer;
    document.getElementById('fr-feedback').textContent = isCorrect ? 'Correct!' : 'Incorrect';
    answerField.style.backgroundColor = isCorrect ? 'green' : 'red';
}
