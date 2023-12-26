function checkFreeResponse() {
    // Assuming your free response input field has an ID 'fr-answer'
    const answerField = document.getElementById('fr-answer');
    const userAnswer = answerField.value.trim().toLowerCase();

    const correctAnswer = 'kashyyyk'; // Correct answer
    const feedback = document.getElementById('fr-feedback');

    // Validate answer and provide feedback
    if(userAnswer === correctAnswer) {
        answerField.classList.add('correct');
        answerField.classList.remove('incorrect');
        feedback.textContent = 'Correct!';
        feedback.style.color = 'green';
    } else {
        answerField.classList.add('incorrect');
        answerField.classList.remove('correct');
        feedback.textContent = 'Incorrect';
        feedback.style.color = 'red';
    }
}

// Assuming you have a button for submitting the free response that calls this function
