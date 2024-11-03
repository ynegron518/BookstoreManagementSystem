// Load inventory
async function loadInventory() {
    try {
        const response = await fetch('/api/inventory');
        if (!response.ok) throw new Error('Failed to load inventory');

        const text = await response.text(); // Get the response as plain text
        const books = text.trim().split('\n').map(line => {
            const [title, author, edition, genre, quantity, price] = line.split('|');
            return { title, author, edition, genre, quantity, price };
        });

        const inventoryList = document.getElementById("inventoryList");
        inventoryList.innerHTML = books.map(book => `
            <div class="book-item">
                <p><strong>Title:</strong> ${book.title}</p>
                <p><strong>Author:</strong> ${book.author}</p>
                <p><strong>Edition:</strong> ${book.edition}</p>
                <p><strong>Genre:</strong> ${book.genre}</p>
                <p><strong>Quantity:</strong> ${book.quantity}</p>
                <p><strong>Price:</strong> $${parseFloat(book.price).toFixed(2)}</p>
            </div>
        `).join('');
    } catch (error) {
        console.error('Error loading inventory:', error);
        document.getElementById("inventoryList").innerHTML = '<p class="error">Failed to load inventory</p>';
    }
}

// Search books
async function searchBooks() {
    const searchTerm = document.getElementById("searchInput").value.trim();
    try {
        const response = await fetch(`/api/search?term=${encodeURIComponent(searchTerm)}`);
        if (!response.ok) throw new Error('Search failed');
        
        const text = await response.text(); // Use plain text
        document.getElementById("inventoryList").innerHTML = text || '<p class="error">No results found</p>';
    } catch (error) {
        console.error('Search error:', error);
    }
}

// Place an order
async function placeOrder(event) {
    event.preventDefault();
    const title = document.getElementById("orderTitle").value;
    const quantity = document.getElementById("orderQuantity").value;
    
    try {
        const response = await fetch('/api/place_order', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: new URLSearchParams({ title, quantity })
        });
        
        const statusElement = document.getElementById("orderStatus");
        statusElement.innerText = await response.text();
        statusElement.className = response.ok ? 'success' : 'error';
        
        if (response.ok) {
            loadInventory(); // Refresh inventory after successful order
            event.target.reset();
        }
    } catch (error) {
        console.error('Order error:', error);
        document.getElementById("orderStatus").innerText = 'Failed to place order';
    }
}

// Add a new book
async function addBook(event) {
    event.preventDefault();
    const bookData = {
        title: document.getElementById("bookTitle").value,
        author: document.getElementById("bookAuthor").value,
        edition: parseInt(document.getElementById("bookEdition").value),
        genre: document.getElementById("bookGenre").value,
        quantity: parseInt(document.getElementById("bookQuantity").value),
        price: parseFloat(document.getElementById("bookPrice").value)
    };

    try {
        const response = await fetch('/api/add_book', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: new URLSearchParams(bookData)
        });
        
        const statusElement = document.getElementById("addBookStatus");
        statusElement.innerText = await response.text();
        statusElement.className = response.ok ? 'success' : 'error';
        
        if (response.ok) {
            loadInventory();
            event.target.reset();
        }
    } catch (error) {
        console.error('Add book error:', error);
        document.getElementById("addBookStatus").innerText = 'Failed to add book';
    }
}
